/*************************************************************************
    > File Name: poll.cpp
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年04月19日 星期四 19时52分12秒

	int poll(struct pollfd *fds, nfds_t nfds, int timeout);
	// 成功返回 个数, 失败, 返回 -1, 超时返回 0

	struct pollfd 
	{
 		int   fd;         	// 文件描述符 
  		short events;     	// 监听的事件，比如可读事件，可写事件 
  		short revents;    	// poll 函数的返回结果，是可读还是可写 
	};

	events 参数 : 
		POLLIN: 监听是描述符是否可读，相当于 select 中的读集合参数。
		POLLPRI：监听是否有紧急数据可读（比如 TCP 套接字上的 out-of-band(OOB) 数据，这种我们还没学，所以不考虑监听它了）
		POLLOUT：监听是描述符是否可写。
		POLLERR：这种情况极少见，很抱歉后面的实验没演示。一般是硬件上的问题吧，可以参考这个帖子传送门。
		POLLHUP：对端挂断，比如对于有名管道，其中一端关闭了。
		POLLNVAL：使用了未打开的描述符


 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
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
	char buf[N], line[N];

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

	struct pollfd fd_poll[NUM];
	for(int i = 0; i < NUM; i++)
	{
		fd_poll[i].fd = fd[i];
		fd_poll[i].events = POLLIN;
	}

	int num;
	while(1)
	{
		if((num = poll(fd_poll, NUM, -1)) < 0)
			write_error("poll error");
		else if(num == 0)
			printf("poll is 0\n");

		for(int i = 0; i < NUM; i++)
		{
			if(fd_poll[i].revents == POLLIN)
			{
				num = read_fd(fd_poll[i].fd);
				if(num == 0)
					fd_poll[i].fd = -1;
			}
			if(fd_poll[i].revents == POLLNVAL)
				printf("POLLNVAL\n");
		}

	}



	exit(EXIT_SUCCESS);
}

