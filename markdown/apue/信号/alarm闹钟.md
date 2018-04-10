# alarm闹钟

标签（空格分隔）： 信号

---

### alarm理解

```
#include <unistd.h>

unsigned int alarm(unsigned int seconds)
// 返回值 : 0 或者 剩余的时间. 如果在seconds秒内再次调用了alarm函数设置了新的闹钟，则后面定时器的设置将覆盖前面的设置，即之前设置的秒数被新的闹钟时间取代；当参数seconds为0时，之前设置的定时器闹钟将被取消，并将剩下的时间返回。
```
alarm函数可以设置一个定时器, 在将来的某个时刻调用.
**每个程序只能有一个闹钟时间, 当然, 不是指只有一个alarm()函数调用.**

```c++
/*************************************************************************
    > File Name: alarm_test_1.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月04日 星期日 16时30分19秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static void sig_alarm(int);

int main(void)
{
	if(signal(SIGALRM, sig_alarm) == SIG_ERR)
		fprintf(stderr, "signal(SIGALRM) error");

	alarm(2);
	sleep(1);

	printf("time out\n");

	exit(0);
}

void sig_alarm(int)
{
	printf("alarm time out!\n");

	exit(0);
}
```

运行结果
```linux
[root@localhost Signal]# ./a.out 
time out
```
在此可以看出, 调用alarm()函数调用后, 程序会继续执行, 而不会像sleep()一样阻塞. 因为alarm()如果阻塞, 那么当2s过了会调用sig_alarm()函数, 而不是再执行sleep()了.


*验证*
```c++
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static void sig_alarm(int);

int main(void)
{
	if(signal(SIGALRM, sig_alarm) == SIG_ERR)
		fprintf(stderr, "signal(SIGALRM) error");

	alarm(2);
	printf("hello\n");
	sleep(4);

	printf("time out\n");

	exit(0);
}

void sig_alarm(int)
{
	printf("alarm time out!\n");

	exit(0);
}
```

运行结果
``` c++
[root@localhost Signal]# ./a.out 
hello
alarm time out!
```

先输出了hello, 然后执行了sleep(), 然而alarm()时间只是2s, 所以2s后触发了sig_alarm()函数, 最后退出, 所以sleep之后的语句就无法执行.

*继续验证*

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static void sig_alarm(int);

int main(void)
{
	if(signal(SIGALRM, sig_alarm) == SIG_ERR)
		fprintf(stderr, "signal(SIGALRM) error");

	alarm(4);
	for(int i = 0; ; i++)
	{
		printf("%d\n", i);
		sleep(1);
	}

	exit(0);
}

void sig_alarm(int)
{
	printf("alarm time out!\n");

	exit(0);
}
```

运行结果
```c++
[root@localhost Signal]# ./a.out 
0
1
2
3
alarm time out!
```

- - -

### sleep改写

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

static jmp_buf env_jmp;
static void sig_alrm(int)
{
	longjmp(env_jmp, 1);
}

unsigned int sleep2(unsigned int seconds)
{
	if(signal(SIGALRM, sig_alrm) == SIG_ERR)
		return (seconds);

	if(setjmp(env_jmp) == 0)
	{
		alarm(seconds);
		// 等待alarm()结束才能往下执行其他的, 进程阻塞
		pause();
	}

	return alarm(0);
}

int main(void)
{
    sleep2(2);
    
    exit(0);
}
```

- - -

### 一个read计时程序

用alarm()定时一个read计时器, 当达到时间后就停止输入, 不一直等待输入. 
在shell编程里面, **"read -t sec" 表示sec的时间结束输入**

```c
#include <stdio.h>
#include <setjmp.h>
#include "apue.h"
#include <signal.h>

static void sig_alarm(int);
static jmp_buf env_alarm;

int main(void)
{
	int n;
	char line[MAXLINE];

	if(signal(SIGALRM, sig_alarm) == SIG_ERR)
		err_sys("signal(SIGALRM) error");

	if(setjmp(env_alarm) != 0)
	{
		err_quit("read timeout");
		pause();
	}
 
    // 计时, read()时间等待2s
	alarm(2);
	if((n = read(STDIN_FILENO, line, MAXLINE)) < 0)
		err_sys("read error");
	// 时间到了, alarm(0)返回0, 时间不到就返回剩余时间.
	alarm(0);

	write(STDOUT_FILENO, line, n);

	exit(0);
}

static void sig_alarm(int)
{
	printf("time out\n");
 	longjmp(env_alarm, 1);
}
```

