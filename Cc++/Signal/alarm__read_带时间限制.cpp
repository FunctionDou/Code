/*************************************************************************
    > File Name: alarm__read_带时间限制.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月02日 星期五 22时18分21秒

	alarm只设定一个闹钟，时间到达并执行其注册函数之后，闹钟便失效。如果想循环设置闹钟，需在其注册函数中在调用alarm函数。
	void Fun(int)
	{
	    signal(SIGALRM, handler); //让内核做好准备，一旦接受到SIGALARM信号,就执行 Fun()
	    alarm(5);
	}

 ************************************************************************/

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
	
	alarm(0);
	alarm(2);

	exit(0);
}

static void sig_alarm(int)
{
	printf("time out\n");
 	longjmp(env_alarm, 1);
}
