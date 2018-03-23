/*************************************************************************
    > File Name: t.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月05日 星期一 20时24分05秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

void *fun1(void)
{
	pthread_exit((void *)1);
}

void *fun2(void)
{
	return ((void *)3);
}

void *thread_fun1(void *)
{
	fun1();

	return ((void *)0);
}

void *thread_fun2(void *)
{
	fun2();

	pthread_exit((void *)2);
}


int main(void)
{
	void *tret;
	pthread_t tid1, tid2;
	
	 // 建立第一个线程
	if((errno = pthread_create(&tid1, NULL, thread_fun1, NULL)) != 0)
		fprintf(stderr, "pthread_create error : %s\n", strerror(errno));

	if((errno = pthread_join(tid1, &tret)) != 0)
		fprintf(stderr, "pthread_join error : %s\n", strerror(errno));
	// 第一个的返回码
	printf("tid1 : %ld\n", (long)tret);

	// 建立第二个线程
	if((errno = pthread_create(&tid2, NULL, thread_fun2, NULL)) != 0)
		fprintf(stderr, "pthread_create error : %s\n", strerror(errno));

	if((errno = pthread_join(tid2, &tret)) != 0)
		fprintf(stderr, "pthread_join error : %s\n", strerror(errno));
	// 第二个的返回码
	printf("tid2 : %ld\n", (long)tret);

	exit(0);
}
