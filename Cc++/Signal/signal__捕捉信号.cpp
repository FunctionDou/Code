/*************************************************************************
    > File Name: signal__捕捉信号.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月06日 星期二 19时58分52秒

******************************  signal  *********************************
	#include <signal.h>
	
	void (* signal(int signo, void(*func)(int)))(int);
	
	错误返回SIG_ERR

	signo : 一个信号, 像SIGINT(终止信号), SIGALRM(超时信号)等.
		SIG_IGN : 像内核表示忽略该信号(SIGKILL, SIGSTOP不能忽略)
		SIG_DFL : 系统默认的信号动作
		SIG_ERR : 信号错误
	
	void(*func)(int) : 一个传入int类型, 无返回值的函数

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
