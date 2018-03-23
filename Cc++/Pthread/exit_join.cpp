/*************************************************************************
    > File Name: exit_join.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月07日 星期三 17时24分08秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include "apue.h"

void *Fun1(void *)
{
	printf("create 1\n");
	return ((void *)1);
}

void *Fun2(void *)
{
	printf("create 2\n");
	pthread_exit((void *)2);
}

int main(void)
{
	int err;
	pthread_t pthreadid1, pthreadid2;
	void *tmp;

	err = pthread_create(&pthreadid1, NULL, Fun1, NULL);
	if(err != 0)
		err_exit(err, "pthread_create(Fun1) error");
	
	err = pthread_create(&pthreadid2, NULL, Fun2, NULL);
	if(err != 0)
		err_exit(err, "pthread_create(Fun2) error");

	pthread_join(pthreadid1, &tmp);
	printf("create Fun1 exit code %ld\n", (long)tmp);

	pthread_join(pthreadid2, &tmp);
	printf("create Fun2 exit code %ld\n", (long)tmp);

	exit(0);
}
