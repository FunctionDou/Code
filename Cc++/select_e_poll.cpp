/*************************************************************************
    > File Name: select_e_poll.cpp
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年04月27日 星期五 07时52分50秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <sys/epoll.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/stat.h>

#define N 64
int Write(int);

int Write(int fd)
{
	int n;
	char buf[N];
	n = read(fd, buf, sizeof(buf));
	n = write(STDOUT_FILENO, buf, n);

	return n;
}

int main(int argc, char *argv[])
{
	int fd[2];
	fd[0] = open("a.fifo", O_RDONLY);
	fd[1] = open("b.fifo", O_RDONLY);

	int max;
	max = fd[1] + 1;

	/*
	fd_set FD, fd_temp;
	FD_ZERO(&FD);
	FD_SET(fd[0], &FD);
	FD_SET(fd[1], &FD);

	if(select(max, &FD, NULL, NULL, NULL) < 0)
		exit(1);
	
	fd_temp = FD;
	for(int i = 0; i < max; i++)
	if(FD_ISSET(fd[0], &fd_temp))
		Write(fd[0]);
	else if(FD_ISSET(fd[1], &fd_temp))
		Write(fd[1]);

	FD_CLR(fd[0], &FD);
	FD_CLR(fd[1], &FD);
	*/

	
	/*
	struct pollfd fd_poll[2];
	for(int i = 0; i < 2;i++)
	{
		fd_poll[i].events = POLLIN;
		fd_poll[i].fd = fd[i];
	}

	if(poll(fd_poll, max, -1) < 0)
		exit(1);

	for(int i = 0; i < max; i++)
	{
		printf("fd : ");
	if(fd_poll[0].revents == POLLIN)
	{
		printf("fd[0] :");
		Write(fd_poll[0].fd);
	}
	else if(fd_poll[1].revents == POLLIN)
	{
		printf("fd[1] : ");
		Write(fd_poll[1].fd);
	}
	}
	*/

	int epfd;
	epfd = epoll_create(2);
	for(int i = 0; i < 2; i++)
	{
		struct epoll_event event;
		event.data.fd = fd[i];
		event.events = EPOLLIN;
		if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd[i], &event) < 0)
		{
			printf("error\n");
			exit(1);
		}
	}

	struct epoll_event eves[2];
	for(int i = 0; i < 3; i++)
	{
		printf("epoll");
		if(epoll_wait(epfd, eves, 3, -1) < 0)
			exit(1);
		if(eves[0].events == EPOLLIN)
			Write(eves[0].data.fd);
		if(eves[1].events == EPOLLIN)
			Write(eves[1].data.fd);
	}
	
	close(fd[0]);
	close(fd[1]);

	exit(0);
}
