/*************************************************************************
    > File Name: z.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月03日 星期六 22时55分58秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static void Fun(sigset_t sigset)
{
	if(sigismember(&sigset, SIGSEGV))
		printf("SIGEGV\n");
	if(sigismember(&sigset, SIGINT))
		printf("SIGINT\n");
	if(sigismember(&sigset, SIGTERM))
		printf("SIGTERM\n");
	if(sigismember(&sigset, SIGALRM))
		printf("SIGALRM\n");
}

int main()
{
	sigset_t new_sigset, old_sigset;
	sigemptyset(&new_sigset);
	sigemptyset(&old_sigset);
	
	sigaddset(&new_sigset, SIGSEGV);
	sigaddset(&new_sigset, SIGINT);
	sigaddset(&new_sigset, SIGTERM);
	printf("1 new_sigset\n");
	Fun(new_sigset);
	printf("\n\n");

	sigprocmask(SIG_BLOCK, &new_sigset, &old_sigset);
	printf("2 old_sigset\n");
	Fun(old_sigset);
	printf("\n\n");
	printf("3 new_sigset\n");
	Fun(new_sigset);
	printf("\n\n");

	
	// sigaddset(&old_sigset, SIGALRM);
	sigaddset(&new_sigset, SIGALRM);
	sigprocmask(SIG_BLOCK, &new_sigset, &old_sigset);
	printf("4 old_sigset\n");
	Fun(old_sigset);
	printf("\n\n");
	printf("5 new_sigset\n");
	Fun(new_sigset);
	printf("\n\n");

	// 取消new_sigset中的信号屏蔽, 此时程序的信号集为NULL
	sigprocmask(SIG_UNBLOCK, &new_sigset, &old_sigset);
	printf("6 old_sigset\n");
	Fun(old_sigset);
	printf("\n\n");
	printf("7 new_sigset\n");
	Fun(new_sigset);
	printf("\n\n");

	// 这里old_sigset信号集中的信号为NULL, 因为上次程序的屏蔽字为NULL
	sigprocmask(SIG_SETMASK, &new_sigset, &old_sigset);
	printf("8 old_sigset\n");
	Fun(old_sigset);
	printf("\n\n");
	printf("9 new_sigset\n");
	Fun(new_sigset);
	printf("\n\n");
	
	exit(0);
}
