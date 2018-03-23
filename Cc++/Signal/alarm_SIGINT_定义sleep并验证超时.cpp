/*************************************************************************
    > File Name: alarm_SIGINT.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月03日 星期六 18时31分29秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <signal.h>
#include <unistd.h>
#include <setjmp.h>

static jmp_buf env_jmp;

static void sig_int(int);

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
		pause();
	}

	return alarm(0);
}

static void sig_int(int)
{
	int i, j;
	volatile int k = 0;

	printf("\nsig_int starting\n");
	for(i = 0; i < 300000; i++)
		for(j = 0; j < 4000; j++)
			k += i*j;
	printf("sig_int finished\n");

}

int main(void)
{
	unsigned int unslept;
	
	if(signal(SIGINT, sig_int) == SIG_ERR)
		err_sys("signal(SEGINT) error");
	
	unslept = sleep2(3);
	printf("sleep2 returned %u\n", unslept);

	exit(0);
}

