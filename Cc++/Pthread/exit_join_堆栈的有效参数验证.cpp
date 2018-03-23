/*************************************************************************
    > File Name: exit_join_有效参数验证.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月07日 星期三 17时41分34秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include "apue.h"

struct Foo
{
	int a, b, c;
};

void print(struct Foo *fp)
{
	printf("structure at 0x%lx\n", (unsigned long)fp);
	printf("a = %d\nb = %d\nc = %d\n", fp->a, fp->b, fp->c);
}

void *thr_fun1(void *)
{
	struct Foo foo = { 1, 2, 3, };
	print(&foo);
	
	pthread_exit((void *)1);
}

void *thr_fun2(void *)
{
	printf("create thread ID is %ld\n", (long)pthread_self());

	pthread_exit((void *)2);
}


int main(void)
{
	int err;
	pthread_t pthreadid1, pthreadid2;
	struct Foo foo;
	print(&foo);

	err = pthread_create(&pthreadid1, NULL, thr_fun1, NULL);
	if(err != 0)
		err_exit(err, "create pthread error");
	sleep(1);

	err = pthread_create(&pthreadid2, NULL, thr_fun2, NULL);
	if(err != 0)
		err_exit(err, "create pthread error");
	sleep(1);
	print(&foo);


	exit(0);
}
