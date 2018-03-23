/*************************************************************************
    > File Name: create_2_创建线程.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月05日 星期一 19时34分03秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

void print_id(const char *s)
{
	pthread_t tid;
	pid_t pid;

	pid = getpid();
	tid = pthread_self();

	printf("%s pid = %d, tip = %lu\n", s, pid, (unsigned long)tid);
}

void *thread_print(void *)
{
	print_id("new thread :");

	return ((void *)0);
}

int main(void)
{
	pthread_t tid;

	errno = pthread_create(&tid, NULL, thread_print, NULL);
	if(errno != 0)
		fprintf(stderr, "%s", strerror(errno));
	
	sleep(1);
	print_id("main thread :");


	exit(0);
}
