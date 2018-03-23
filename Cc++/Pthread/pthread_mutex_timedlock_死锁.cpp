/*************************************************************************
    > File Name: pthread_mutex_timedlock.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月08日 星期四 12时10分23秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include "apue.h"

int main(void)
{
	int err;
	struct timespec tout;
	struct tm *tmp;
	char buf[64];
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&lock);
	printf("mutex is lock\n");
	clock_gettime(CLOCK_REALTIME, &tout);
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	printf("crrent time is %s\n", buf);

	tout.tv_sec += 10;
	err = pthread_mutex_timedlock(&lock, &tout);
	clock_gettime(CLOCK_REALTIME, &tout);
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	printf("the time is now %s\n", buf);
	if(err == 0)
		printf("mutex locked again\n");
	else
		printf("can't lock mvtex again:%s\n", strerror(err));

	exit(0);
}
