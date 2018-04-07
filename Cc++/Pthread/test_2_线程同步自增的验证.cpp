/*************************************************************************
    > File Name: t.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月06日 星期二 08时59分30秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

static volatile int a;

void *Fun1(void *)
{
	sleep(1);

	for(int i = 0; i < 100; i++)
		a++;

	pthread_exit((void *)0);
}

void *Fun2(void *)
{
	sleep(1);

	for(int i = 0; i < 100; i++)
		a++;

	pthread_exit((void *)0);
}

int main(void)
{
	pthread_t tid1, tid2;
	a = 0;
	
	if((errno = pthread_create(&tid1, NULL, Fun1, NULL)) != 0)
		fprintf(stderr, "pthread_create error : %s", strerror(errno));

	if((errno = pthread_create(&tid2, NULL, Fun2, NULL)) != 0)
		fprintf(stderr,"pthread_create error : %s", strerror(errno));

	sleep(2);
	printf("a = %d\n", a);
	

	exit(0);
}
