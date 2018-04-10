/*************************************************************************
    > File Name: sem_init_1_汽车问题_实现线程的竞争与同步.cpp
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年03月22日 星期四 17时20分56秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>

// 车上人数
static int num;
// 信号量
static sem_t sem;
// 线程调用的函数
void *Boarding(void *board);
void *Get_off(void *off);

// 线程创建的函数
void Pthread_create(pthread_t *t, void *(*fun)(void *), void *arg);
/// write函数
int Write(const char *s);

int main()
{
	srand(0);
	num = 0;
	pthread_t thread_father, thread_child;
	// 初始化信号量
	sem_init(&sem, 0, 0);

	// 创建两个线程
	Pthread_create(&thread_father, Boarding, NULL);
	Pthread_create(&thread_child, Get_off, NULL);

	// 等待两个线程
	pthread_join(thread_father, NULL);
	pthread_join(thread_child, NULL);

	exit(0);
}

// 创建线程
void Pthread_create(pthread_t *t, void *(*fun)(void *), void *arg)
{
	if ((errno = pthread_create(t, NULL, fun, arg)) != 0)
	{
		fprintf(stderr, "pthread_create error : %s\n", strerror(errno));
		exit(1);
	}
}

// 创建写操作
int Write(const char *s)
{
	int n;
	n = strlen(s);
	if (write(STDOUT_FILENO, s, n) != n)
	{
		fprintf(stderr, "write error\n");
		exit(1);
	}

	return n;
}

// 上车
void *Boarding(void *board)
{
	static int t;
	// 上车总次数的生成
	t = rand() % 5 + 3;
	while (t--)
	{
		// 人数为 0 : sv == 0, 只能上车, 阻塞下车
		if (num == 0)
			sem_post(&sem);
		num = rand() % 2 + 1;
		Write("Board\n");
		usleep(1000 * 10);
	}

	// 解锁
	pthread_exit((void *)0);
}

void *Get_off(void *off)
{
	static int tt;
	// 下车次数的随机次数
	tt = rand() % 4 + 4;
	int n;
	while (tt--)
	{
		// 人数为 0. 下车阻塞, 只能上车
		if (num == 0)
			sem_wait(&sem);
		n = rand() % 5;
		num = num - n ? num - n : 0;
		Write("get off\n");
		//sleep(1);
		usleep(1000 * 10);
	}

	pthread_exit((void *)0);
}
