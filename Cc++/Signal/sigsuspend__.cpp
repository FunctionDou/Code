/*************************************************************************
    > File Name: sigsuspend__.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月06日 星期二 14时27分10秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

// static void pr_mask(char const *ptr);
//
#include <errno.h>

void pr_mask(const char *str)
{
    sigset_t sigset;
    int errno_save;

    errno_save = errno;

    printf("%s\n", str);
    if(sigismember(&sigset, SIGINT))
        printf("SIGINT\t");
    if(sigismember(&sigset, SIGQUIT))
        printf("SIGQUIT\t");
    if(sigismember(&sigset, SIGUSR1))
        printf("SIGUSR1\t");
    if(sigismember(&sigset, SIGALRM))
        printf("SIGALRM\t");
}

static void sig_int(int);

int main(void)
{
	sigset_t newmask, oldmask, waitmask;

	pr_mask("program start: ");

	if(signal(SIGINT, sig_int) == SIG_ERR)
		err_sys("signal(SIGINT) error");

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGINT);
	sigemptyset(&waitmask);
	sigaddset(&waitmask, SIGUSR1);

	if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
		err_sys("sigprocmask(SIG_BLOCK) error");
	pr_mask("in critical region");

	if(sigsuspend(&waitmask))
	pr_mask("after return from sigsuspend : ");

	if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
		err_sys("sigprocmask(SIG_SETMASK) error");
	pr_mask("program exit : ");



	exit(0);
}

static void sig_int(int)
{
	pr_mask("\nin sig_int: ");
}

/* 
static void pr_mask(char const *ptr)//打印当前信号屏蔽字的屏蔽信号
{
	sigset_t                sigset;
	int                     errno_save;
	int                     ret;
	errno_save = errno;
    ret = sigprocmask(0, NULL, &sigset);
	
	if(ret == -1)
	{
		perror("sigprocmask error");
        exit(1);
    }

	printf("%s", ptr);


    if(sigismember(&sigset, SIGINT))
	{
        printf("SIGINT ");
	}

	if(sigismember(&sigset, SIGQUIT))
	{
                printf("SIGQUIT ");
	}

    if(sigismember(&sigset, SIGUSR1))
	{
            printf("SIGUSR1 ");
    }

    if(sigismember(&sigset, SIGALRM))
	{
		printf("SIGALRM ");
	}

	printf("\n");

    errno = errno_save;
}*/ 
