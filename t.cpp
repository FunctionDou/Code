/*************************************************************************
    > File Name: t.cpp
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年04月19日 星期四 20时25分45秒
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
#include <poll.h>
#include <sys/epoll.h>

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
	if(create_fifo("a.fifo") < 0)
		write_error("create a.fifo error");
	if(create_fifo("b.fifo") < 0)
		write_error("create b.fifo error");
	
	struct sigaction action;
	action.sa_handler = sig_sigint;
	sigaction(SIGINT, &action, NULL);

	int fd[2], i;
	fd[0] = open("a.fifo", O_RDONLY);
	fd[1] = open("b.fifo", O_RDONLY);
	
	int epfd;
	if((epfd = epoll_create(NUM)) == -1)
		write_error("epoll create error");
	for(i = 0; i < NUM; i++)
	{
		struct epoll_event event;
		event.events = EPOLLIN;
		event.data.fd = fd[i];
		epoll_ctl(epfd, EPOLL_CTL_ADD, fd[i], &event);
	}
	
	struct epoll_event re_event[NUM];
	while(1)
	{
		if(epoll_wait(epfd, re_event, NUM + 1, -1) == -1)
			write_error("epoll wait error");
		for(i = 0; i < NUM; i++)
		{
			if(re_event[i].events == EPOLLIN)
				read_fd(re_event[i].data.fd);
		}
	}

	exit(EXIT_SUCCESS);
}

