## 关于exit与return的区别

<font color=#b20>**return 会返回栈, 释放局部变量, 弹出栈.**</font>

> **下列程序的问题**
>
> 1. 当调用vfork()函数, 因为vfork函数的特点就是子进程与父进程共享数据, 栈等, 而且子进程先执行, 父进程阻塞.
> 2. 所以当子进程调用结束调用return, 会将进程的栈弹出, 那么父进程执行的时侯, 就会发现已经没有栈了, 所以就会抛出一个错误.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	int num = 0;

	if((pid = vfork()) < 0)
		return 0;
	else if(pid == 0)
	{
		num++;
		printf("hello world! %d\n", num);

		return 0;
	}
	sleep(1);
	printf("hello world%d\n",num);

	return 0;
}
```

结果输出, 报错

```c
rpz@funct:~$ ./a.out 
hello world! 1
hello world32563 
a.out: cxa_atexit.c:100: __new_exitfn: Assertion `l != NULL' failed.
已放弃
```

---

**前面说了return会弹出栈, 并且释放局部变量, 而<font color#b20>使用exit()函数, 他会直接进入内核, 在程序死亡的时侯, 才会为程序释放所有的空间, 内核也就可以回收空间. 这也就知道exit函数并不会调用函数栈, exit是通知进程叫进程自杀. 而且规定了调用vfork绝对不能调用return</font>**

**还有一点, 在c++中调用return会调用析构函数, 而使用exit函数就不会调用析构函数.** 如下 :

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

class Temp
{
  public:
    Temp()
    {
        printf("Temp\n");
    }
    ~Temp()
    {
        printf("~Temp\n");
    }
};

int main(void)
{
    pid_t pid;
    if ((pid = fork()) == 0)
    {
        printf("return\n");
        {
            Temp temp;
            return 0;
        }
    }
    sleep(1);
    printf("exit\n");
    {
        Temp temp;
        exit(0);
    }
}
```

运行结果:

```c
rpz@funct:~/Code$ ./a.out 
return
Temp
~Temp
exit
Temp
```









