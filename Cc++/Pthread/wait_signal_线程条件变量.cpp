/*************************************************************************
    > File Name: t1.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月06日 星期二 13时46分26秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

static pthread_mutex_t mutex;
static pthread_cond_t cond;
static int num;

void *Fun1(void *)
{
	while(1)
	{
		// 加锁
		if((errno = pthread_mutex_lock(&mutex)) != 0)
			fprintf(stderr, "pthread_mutex_lock error : %s\n", strerror(errno));

		// 一直挂起, wait等待
		while(num > 0)
		{
			printf("子进程pthread_cond_wait()触发\n");
			if((errno = pthread_cond_wait(&cond, &mutex)))
				fprintf(stderr, "pthread_cond_wait error : %s\n", strerror(errno));
		}
		printf("子进程运行\n");
		
		if(num == 10)
		{
			printf("子进程中 num = %d\n", num);
			num = 0;
		}

		// 解锁
		if((errno = pthread_mutex_unlock(&mutex)) != 0)
			fprintf(stderr, "pthread_mutex_unlock error : %s\n", strerror(errno));

		sleep(1);
	}

	pthread_exit((void *)0);
}

int main(void)
{
	pthread_t tid1;
	num = 0;
	
	// 分配
	if((errno = pthread_mutex_init(&mutex, NULL)) != 0)
		fprintf(stderr,"pthread_mutex_init error :%s\n", strerror(errno));

	// 创建
	if((errno = pthread_create(&tid1, NULL, Fun1, NULL)) != 0)
		fprintf(stderr, "pthread_create error : %s\n", strerror(errno));

	while(1)
	{
		// 加锁
		if((errno = pthread_mutex_lock(&mutex)) != 0)
			fprintf(stderr, "pthread_mutex_lock error : %s\n", strerror(errno));

		if(num < 10)
			num++;
		printf("num = %d\n", num);

		// 
		if((errno = pthread_mutex_unlock(&mutex)) != 0)
			fprintf(stderr, "pthread_mutex_unlock error : %s\n", strerror(errno));
		
		if(num >= 10)
		{
			printf("主程序调用 pthread_cond_signal 调用Fun1函数\n");
			pthread_cond_signal(&cond);
		}

		sleep(1);

	}

	if((errno = pthread_mutex_destroy(&mutex)) != 0)
		fprintf(stderr, "pthread_mutex_destroy error : %s\n", strerror(errno));
	
	exit(0);
}
