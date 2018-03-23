/*************************************************************************
    > File Name: pthread_create.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月07日 星期三 16时53分41秒

	pthread并非Linux系统的默认库，而是POSIX线程库。在Linux中将其作为一个库来使用，因此加上 -lpthread（或-pthread）以显式链接该库。函数在执行错误时的错误信息将作为返回值返回，并不修改系统全局变量errno，当然也无法使用perror()打印错误信息。

	gcc  -pthread

*******************************  pthread_equal **************************

	#include <pthread.h>

	// 比较两个线程ID是否相等
	// 线程 ID 不一定是一个非负整数,也有可能是一个结构体。所以,要对比两个线程是否相同,必须使用pthread_equal 函数来进行.
	
	int pthread_equal(pthread_t tidl, pthread_t tid2)
	
	// 返回值 : 相等, 返回 !0; 不相等, 返回 0.
	
*******************************  pthread_self ***************************

	pthread_t pthread_self(void)
	
	// 返回线程的ID

*******************************  pthread_create *************************

	int pthread_create(pthread_t *restrict tidp, const pthread_attr_t *restrict attr, void *(*start_rtn)(void *), void *restrict arg)
	
	// 成功, 返回 0; 失败, 返回错误编码
	
	参数:

	tidp : 指向新线程ID的内存单元
	attr : 定制不同属性的线程. (attr = NULL, 默认属性)
	start_rtn : 指向线程执行的函数
	arg : 函数传入的参数. (arg = NULL, 不传入参数)

 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include "apue.h"

pthread_t newid;

void printids(const char *s)
{
	pid_t pid;
	pthread_t pthreadid;

	pid = getpid();
	pthreadid = pthread_self();

	printf("%s is pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid, (unsigned long)pthreadid, (unsigned long)pthreadid);
}

void* thr_fn(void *)
{
	printids("new thread : ");
	return ((void *)0);
}

int main(void)
{
	int err;

	err = pthread_create(&newid, NULL, thr_fn, NULL);
	if(err != 0)
		err_sys("pthread_create error");
	
	sleep(1);
	printids("main thread : ");
	sleep(1);


	exit(0);
}
