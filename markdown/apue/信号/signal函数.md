# signal函数

标签（空格分隔）： 信号

---

信号是软件中断.
```
#include <signal.h>
void (* signal(int signo, void(*func)(int)))(int);
// 看起来确实听恐怖的, 不过就是两个参数, 下面来说说具体的参数
// 错误返回SIG_ERR
```
> 1. signo : 一个信号, 像SIGINT(终止信号), SIGALRM(超时信号)等.
>* SIG_IGN : 像内核表示忽略该信号(SIGKILL, SIGSTOP不能忽略)
>* SIG_DFL : 系统默认的信号动作
>* SIG_ERR : 信号错误
> 2. void(*func)(int) : 一个传入int类型, 无返回值的函数


``` c++
/*************************************************************************
    > File Name: signal__捕捉信号.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月06日 星期二 19时58分52秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <signal.h>

static void sig_usr(int);

int main(void)
{
	if(signal(SIGUSR1, sig_usr) == SIG_ERR)
		err_sys("signal(SIGUSR1) error");
	if(signal(SIGINT, sig_usr) == SIG_ERR)
		err_sys("signal(SIGINT) error");

	// 一直等待, 使进程挂起, 直到捕捉到一个信号为止
	for( ; ; )
		pause();

	exit(0);
}

void sig_usr(int signo)
{
	if(signo == SIGUSR1)
		printf("SIGUSR1\n");
	if(signo == SIGINT)
		printf("SIGINT\n");
	else
		printf("signal error");
}


/*   样例

[root@localhost Signal]# ./a.out &
[2] 10786
[root@localhost Signal]# kill -USR1 10786
SIGUSR1
[root@localhost Signal]# ./a.out 
^CSIGINT
^CSIGINT
^CSIGINT

*/
```
当然有一点, 在exec()中信号捕捉是没有意义的, 而在fork()中信号捕捉是有意义的.

### 可靠与不可靠信号

> 可靠信号 : 当前的信号数大于处理信号的速度, 未处理的信号会进入队列, 等待处理, 信号不会丢失.
> 不可靠信号 : 当其信号数大于处理信号的速度, 未处理的信号会忽略, 丢弃. 当发生阻塞时, 产生的信号将丢弃.

**可靠信号不会丢弃信号, 全部处理. 不可靠信号会丢弃多余的信号**

### 中断与重入

中断 : 系统遇到紧急处理事件, 暂时中止CPU现在运行的程序, 等待处理完了再返回进行中止的程序.但是, 有些程序中断之后就不能恢复在接着进行.

    一个可重入的函数简单来说，就是：可以被中断的函数。

就是说，你可以在这个函数执行的任何时候中断他的运行，在任务调度下去执行另外一段代码而不会出现什么错误。
而不可重入的函数由于使用了一些系统资源，比如全局变量区，中断向量表等等，所以他如果被中断的话，可能出现问题，所以这类函数是 不能运行在多任务环境下的。

> 1. 什么情况下是不可重入的呢?
> （1）函数体内使用了静态的数据结构, 使用全局变量；
> （2）函数体内调用了malloc()或者free()函数；
> （3）函数体内调用了标准I/O函数。
> * **保证能够中断是安全的函数**