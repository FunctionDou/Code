/*************************************************************************
    > File Name: epoll.cpp
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年04月19日 星期四 20时25分45秒

epoll 函数:
	// 创建一个 epoll 对象（epoll instance），同时返回该对象的描述符。
	int epoll_create(int size);
	int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
	int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

epoll_event 结构体:
	typedef union epoll_data 
	{
  		void        *ptr;
  		int          fd;
  		uint32_t     u32;
  		uint64_t     u64;
	} epoll_data_t;
	struct epoll_event 
	{
		uint32_t     events;     	// Epoll 事件 
		epoll_data_t data;        // 用户数据 
	};
	events :
		EPOLLIN	监听 fd 是否可读
		EPOLLOUT	监听 fd 是否可写
		EPOLLRDHUP	Linux 2.6.17 后可用。监听流式套接字对象是否关闭或半关闭
		EPOLLPRI	监听是否有紧急数据可读
		EPOLLET	表示设置关联的描述符 IO 事件触发模式

op 方法:
	EPOLL_CTL_ADD	将参数 fd 指定的描述符添加到 epoll 对象中，同时将其关联到一个 epoll 事件对象——即参数 event 所指定的值
	EPOLL_CTL_MOD	修改描述符 fd 所关联的事件对象 event，前提是该 fd 已经添加到了 epoll 对象中
	EPOLL_CTL_DEL	将描述符 fd 从 epoll 对象中移除，此时参数 event 被忽略，也可指定为 NULL



 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

#define N 64
#define NUM 2

int create_fifo(const char *);
void sig_sigint(int);
void write_error(const char *);
int read_fd(int);
int Write(const char *);

int Write(const char *s)
{
	int n;
	n = write(STDOUT_FILENO, s, strlen(s));

	return n;
}

int read_fd(int fd)
{
	int n;
	// 验证边沿触发和水平缓冲的区别, 所以将 buf 设置为 4
	char buf[4], line[N];

	// 水平缓冲使用, 边沿触发使用就能看出区别
	n = read(fd, buf, sizeof(buf));
	if(n > 0)
	{
		sprintf(line, "%d say : %s\n", fd, buf);
		puts(line);
	}
	if(n == 0)
	{
		sprintf(line, "%d is close\n", fd);
		return 0;
	}
	if(n < 0)
		write_error("read error\n");

	return n;
}


void write_error(const char *s)
{
	Write(s);

	exit(EXIT_FAILURE);
}

void sig_sigint(int)
{
	unlink("a.fifo");
	unlink("b.fifo");

	exit(EXIT_FAILURE);
}

int create_fifo(const char *s)
{
	int fifo;
	if((fifo = mkfifo(s, 0644)) < 0)
		return -1;
	
	return 0;
}

int main()
{
	if(create_fifo("a.fifo") == -1)
		write_error("create mkfifo error\n");
	if(create_fifo("b.fifo") == -1)
		write_error("create mkfifo error\n");

	struct sigaction action;
	action.sa_handler = sig_sigint;
	if(sigaction(SIGINT, &action, NULL) == -1)
		write_error("sigaction error");

	int fd[NUM];
	fd[0] = open("a.fifo", O_RDONLY);
	fd[1] = open("b.fifo", O_RDONLY);

	int epfd;
	if((epfd = epoll_create(3)) == -1)
		write_error("epoll error");
	for(int i = 0; i < NUM; i++)
	{
		struct epoll_event epoll;
		epoll.data.fd = fd[i];
		// 水平触发模式 : 缓冲区有数据就会输出, 直到缓冲区读取完毕.
		// epoll.events = EPOLLIN;
		epoll.events = EPOLLIN | EPOLLET;
		// 边沿触发 : 当缓冲区有改变的时侯才会立马触发 epoll_wait(), 而不用等到把值写到缓冲区中.
		if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd[i], &epoll) == -1)
			write_error("epoll_ctl error");
	}

	int n;
	struct epoll_event re_event[NUM];
	while(1)
	{
		// 这里的等待数量一定是最大的描述符 + 1
		if((n = epoll_wait(epfd, re_event, NUM + 1, -1)) == -1)
			write_error("epoll wait erro");
		for(int i = 0; i < NUM; i++)
		{
			if(re_event[i].events == EPOLLIN)
				read_fd(re_event[i].data.fd);
		}
	}

	exit(EXIT_SUCCESS);
}

