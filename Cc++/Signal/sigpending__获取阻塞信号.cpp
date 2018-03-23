/*************************************************************************
    > File Name: sigpending__获取阻塞信号.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月06日 星期二 12时34分32秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

static void sig_quit(int);
static void sig_int(int)
{

}

static void Fun(sigset_t sigset)
{
	if(sigismember(&sigset, SIGQUIT))
		printf("SIGEGV\n");
	if(sigismember(&sigset, SIGINT))
		printf("SIGINT\n");
	if(sigismember(&sigset, SIGTERM))
		printf("SIGTERM\n");
	if(sigismember(&sigset, SIGALRM))
		printf("SIGALRM\n");
}


int main(void)
{
	sigset_t newmask, oldmask, pendmask;

	if(signal(SIGQUIT, sig_quit) == SIG_ERR)
		err_sys("can't catch SIGQUIT");

	if(signal(SIGINT, sig_int) == SIG_ERR)
		;
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGQUIT);
	sigaddset(&newmask, SIGINT);
 	if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
 	 	err_sys("error sigprocmask");

	
	Fun(newmask);
	Fun(oldmask);

	sleep(5);

	if(sigpending(&pendmask))
		err_sys("sigpending error");
	if(sigismember(&pendmask, SIGQUIT))
		printf("sigpending SIGQUIT\n");

 	if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
 		err_sys("sigprocmask SIG_SETMASK error");
	printf("SIGQUIT unblock\n");

	Fun(newmask);
	Fun(oldmask);

	for(int i = 0; i < 400000; i++)
		for(int j = 0; j < 10000; j++)
			;
// 	sleep(5);


	exit(0);
}

static void sig_quit(int)
{
	printf("caught SIGQUIT\n");
	if(signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		err_sys("can't reset SIGQUIT");
}
