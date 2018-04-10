# exit()与wait*()函数

标签（空格分隔）：进程控制

---
这节的知识我就基本只是把书上的整理了一下， 仅仅只是个人觉得有用的整理了下。

### exit()函数

>* _Exit() : 其目的是为进程提供一种无需运行终止处理程序或信号处理程序而终止的方法
>* _exit() : _exit()由exit()调用,处理UNIX特定的细节
>* 最后一个进程调用return等其他终止的返回值并不作为进程的返回值. 当最后一个进程从启动例程返回时, 该进程以终止状态0返回.

- 终止
|    |
| :----- |
| **不管进程是怎么终止的,  最后都会调用内核的同一段代码. 这段代码会关闭所有的流操作, 释放所使用的存储器.**|
| 在异常终止时, 系统会返回一个异常终止状态.|
| 父进程通过wait()watipid()函数来获得终止状态.|
| 在最后调用exit()时, 转化为终止状态 |
| **在父进程比子进程先终止时，父进程会改变为init进程(也是之前说的init会成为所有孤进程的父进程**(原因 ：在进程终止时， 内核逐个检查所有活动的进程， 以判断是不是正要终止进程的子进程， 如果是， 则该进程的父进程ID改为1.)|
| 由init收养的父进程是不会变为僵尸进程的(僵尸进程 ： 一个已终止， 但是其父进程没有对其做善后处理) |

- - -

### wait()函数

wait()
>1. pid_t wait(int *statloc)
>*  子进程运行时， wait使调用者堵塞
>*  statloc ！= NULL 则进程的终止状态就存放在其所指的单元， statloc == NULL 则不关心其终止状态
>*  当一个子进程终止就立刻返回。

waitpid()
>2. pid_t waitpid(pid_t pid, int *statloc, int options)
>* 可以并不堵塞
>*  可以等待pid指定的进程终止在执行waitpid()的调用者。
>*  statloc ！= NULL 则进程的终止状态就存放在其所指的单元， statloc == NULL 则不关心其终止状态
>*  pid的值
>   pid == -1 等待任意子进程
>   pid > 0 等待 “进程ID == pid” 的进程
>   pid == 0 等待 ”组ID == 调用进程组ID”的任意进程
>   pid < -1 等待 “组ID == |pid|“ 的进程

**如果一个进程fork一个子进程， 但是想等待子进程终止再进行， 也不希望子进程处于僵尸状态直到父进程终止， 这就需要调用两次fork() (通俗一点就是希望父进程在子进程之前终止)**

```
/*************************************************************************
    > File Name: fork_wait_子进程.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月30日 星期二 11时23分27秒

	
	pid_t waitpid(pid_t pid, int *statloc, int options)
	
	可以并不堵塞
	可以等待pid指定的进程终止在执行waitpid()的调用者。
	statloc ！= NULL 则进程的终止状态就存放在其所指的单元， statloc == NULL 则不关心其终止状态
	
	pid的值
	
	pid == -1 等待任意子进程
	pid > 0 等待 “进程ID == pid” 的进程
	pid == 0 等待 ”组ID == 调用进程组ID”的任意进程
	pid < -1 等待 “组ID == |pid|“ 的进程

 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <sys/wait.h>

int main(void)
{
	// pid_t pid = -1;
	pid_t pid;
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
	{
		if((pid = fork()) < 0)
			err_sys("fork error");
		else if(pid > 0)
			exit(0);

		sleep(2);
		printf("second child, parent pid = %ld\n", (long)getppid());
		exit(0);
	}

	if(waitpid(pid, NULL, 0) != pid)
		err_sys("waitpid error");

	exit(0);
}

/*          样例
[root@192 Process]# ./a.out 
[root@192 Process]# second child, parent pid = 1

[root@192 Process]# 
*/

```


| 宏 | 说明 |
|:----:|:----|
|WIFEXITED|正常终止（真）可执行WEXITSTATUS 来获取子进程传递给exit， _exit参数|
|WIFSIGNALED|异常终止（真）可执行WTERMSIG 获取子进程终止的信号编号|
|WIFSTOPPED|暂停子进程的返回的状态（真）WSTOPSIG 获取子进程暂停的信号编号|

```
/*************************************************************************
    > File Name: exit__退出状态.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月31日 星期三 21时40分13秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <sys/wait.h>

void pr_exit(int status)
{
    // 正常终止
	if(WIFEXITED(status))
		printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
	// 异常终止
	else if(WIFSIGNALED(status))
		printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status), 
#ifdef WCOREDUMP
		WCOREDUMP(status) ? " (core file generated)" : "");
#else
		"");
#endif
    // 暂停子进程
	else if(WIFSTOPPED(status))
		printf("child stopped, signal number = %d", WSTOPSIG(status));

}


int main(void)
{
	pid_t pid;
	int status;

    // 1
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
		exit(7);

	if(wait(&status) != pid)
		err_sys("wait error");
	pr_exit(status);

    // 2
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
		abort();
	
	if(wait(&status) != pid)
		err_sys("wait error");
	pr_exit(status);

    // 3
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
		status /= 0;

	if(wait(&status) != pid)
		err_sys("wait error");
	pr_exit(status);


	exit(0);
}

/*      样例
 ./a.out 
normal termination, exit status = 7
abnormal termination, signal number = 6 (core file generated)
abnormal termination, signal number = 8 (core file generated)
*/ 

```

### 竞争条件

竞争条件 ： 当多个进程对共享数据进行某种处理， 但是最后结果又得取决于进程运行的顺序。(通俗一点就是运行还得看系统的调度顺序)

轮询：等待父进程终止在进行子进程。(消耗cpu资源)
```
while(getppid != 1)
    sleep(1);
```

竞争条件的程序
```
/*************************************************************************
    > File Name: TELL_WAIT_竞争条件.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月31日 星期三 22时38分46秒


	int putc(int ch, FILE *stream);
	参数ch表示要输入的位置，参数stream为要输入的流。
	若正确，返回输入的的字符，否则返回EOF。

 ************************************************************************/

#include <stdio.h>
#include "apue.h"

static void charatatime(char *);

int main(void)
{
	pid_t pid;

	// 修改方法2
	// TELL_WAIT();

	// 修改方法1 
	// if((pid = vfork()) < 0)  没有竞争关系了， 因为先进行子进程
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
	{
		// 修改方法2
		// WAIT_PARENT();
		charatatime("one chile\n");
	}
	else
	{
		// 修改方法2
		// TELL_CHILD(pid);
		charatatime("first parent\n");
	}



	exit(0);
}

static void charatatime (char *str)
{
	char *ptr;
	int c;

	setbuf(stdout, NULL);
	for(ptr = str; (c = *ptr++) != NULL; )
		putc(c, stdout);

}


/*          样例
[root@192 Process]# ./a.out 
first oparent
ne chile
[root@192 Process]# ./a.out 
first paronent
e chile
[root@192 Process]# ./a.out 
first parenot
ne chile
*/

```
每次的结果不一样， 因为这个要跟此时的进程顺序有关。
书上的修改方法2运行有问题， 因为那些函数没有包含进去， 所以无法测试。















