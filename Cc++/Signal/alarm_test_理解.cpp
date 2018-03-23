/*************************************************************************
    > File Name: alarm_test_1.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月04日 星期日 16时30分19秒

	调用alarm()函数调用后, 程序会继续执行, 而不会像sleep()一样阻塞.
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static void sig_alarm(int);

int main(void)
{
	if(signal(SIGALRM, sig_alarm) == SIG_ERR)
		fprintf(stderr, "signal(SIGALRM) error");

	alarm(4);
	for(int i = 0; ; i++)
	{
		printf("%d\n", i);
		sleep(1);
	}

	exit(0);
}

void sig_alarm(int)
{
	printf("alarm time out!\n");

	exit(0);
}
