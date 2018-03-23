/*************************************************************************
    > File Name: cleanup_push_pop.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月07日 星期三 18时15分41秒
 ************************************************************************/
#include <stdio.h>
#include <pthread.h>
#include "apue.h"

void cleanup(void *arg)
{
	printf("cleanup : %s\n", (char *)arg);
}

void *thr_fun1(void *arg)
{

	char ch[] = "Fun1 first";
	char ch2[] = "Fun1 second";

	printf("create Fun1\n");
	// 将cleanup的函数入栈
	pthread_cleanup_push(cleanup, ch);
	pthread_cleanup_push(cleanup, ch2);
	printf("thread 1 push complete\n");

	sleep(100);

	if(arg)
		return ((void *)1);
		
	// 将函数出栈
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);

	return ((void *)1);
}

void *thr_fun2(void *arg)
{
	char ch[] = "Fun2 first";
	char ch2[] = "Fun2 second";

	printf("create Fun2\n");
	// 将cleanup的函数入栈
	pthread_cleanup_push(cleanup, ch);
	pthread_cleanup_push(cleanup, ch2);
	printf("thread 2 push complete\n");

	if(arg)
		pthread_exit((void *)2);

    // 将函数出栈
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);

	pthread_exit((void *)2);
}

int main(void)
{
	int err;
	pthread_t pthreadid1, pthreadid2;
	void *tret;

	err = pthread_create(&pthreadid1, NULL, thr_fun1, (void *)1);
	if(err != 0)
		err_exit(err, "create 1 error");
	sleep(1);
	// 取消线程
	pthread_cancel(pthreadid1);

    // 获取返回值
	pthread_join(pthreadid1, &tret);
	printf("thread 1 exit code : %ld\n", (long)tret);

	err = pthread_create(&pthreadid2, NULL, thr_fun2, (void *)1);
	if(err != 0)
		err_exit(err, "creat 2 error");
	// 获取返回值
	pthread_join(pthreadid2, &tret);
	printf("thread 2 exit code : %ld\n", (long)tret);

	exit(0);
}
