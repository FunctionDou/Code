/*************************************************************************
    > File Name: t1.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月04日 星期日 19时41分34秒
 ************************************************************************/
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
		pause();
	}

	return alarm(0);
}

int main(void)
{
    sleep2(2);
    
    exit(0);
}
