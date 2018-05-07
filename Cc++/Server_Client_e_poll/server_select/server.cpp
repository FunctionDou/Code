/*************************************************************************
  > File Name: server.cpp
  > Author: Function_Dou
  > Mail: NOT
  > Created Time: 2018年04月24日 星期二 10时46分55秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/select.h>

#define AS struct sockaddr *
#define N 64

int Listen(int , unsigned short *, char * argv);
void * child_thread(void *);
int Write(int, const char *);
void SIG_HCLD(int);
int FD_Max(int *, fd_set *FD);
void  insert(int, int *);

void insert(int fd, int FD_Max[])
{
	for(int i = 0; i < FD_SETSIZE; i++)
		if(FD_Max[i] == -1)
		{
			FD_Max[i] = fd;
			break;
		}
}

int FD_Max(int *fdmax, fd_set *FD)
{
	int max = 0;
	
	FD_ZERO(FD);
	for (int i = 0; i < FD_SETSIZE; i++)
	{
		if(fdmax[i] != -1)
			FD_SET(fdmax[i], FD);
		if(max < fdmax[i])
			max = fdmax[i];
	}
	
	return max;
}

void SIG_HCLD(int signo)
{
	pid_t pid;

	if(signo == SIGCHLD)
	{
		int stat;
		while(1)
			if((pid = waitpid(-1, &stat, 0)) <= 0)
				break;
	}
	if(signo == SIGPIPE)
	{
		Write(STDOUT_FILENO, "SIGPIPE\n");
		exit(1);
	}
}

int Write(int fd, const char *s)
{
	int n;
	n = write(fd, s, strlen(s));

	return n;
}

void * child_thread(void *fd)
{
	int FD = *(int *)fd;
	char buf[N];
	int n;
	while(1)
	{
		n = read(FD, buf, sizeof(buf));
		if(n > 0)
			write(STDOUT_FILENO, buf, n);
		else if(n == 0)
		{
			sprintf(buf, "%d car\n", FD);
			Write(STDOUT_FILENO, buf);
			pthread_exit((void *)0);
		}
	}

	pthread_exit((void *)0);
}


int Listen(int fd, unsigned short * port, char *argv)
{
	struct sockaddr_in server_addr;
	socklen_t len_address;

	len_address = sizeof(server_addr);
	bzero(&server_addr, len_address);
	// fd = socket(AF_INET, SOCK_STREAM, 0);
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	server_addr.sin_family = AF_INET;
	// server_addr.sin_port = *port;
	server_addr.sin_port = htons(*port);
	// server_addr.sin_addr.s_addr = *port;
	// server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_addr.s_addr = inet_addr(argv);
	if(bind(fd, (struct sockaddr *)&server_addr, len_address) < 0)
		exit(1);

	if(listen(fd, 0) < 0)
		exit(1);

	return fd;
}

int main(int argc, char *argv[])
{
	/*
	struct sigaction action1, action2;
	action1.sa_handler = SIG_HCLD;
	action2.sa_handler = SIG_HCLD;
	if(sigaction(SIGCHLD, &action1, NULL) == -1 && sigaction(SIGPIPE, &action2, NULL) == -1)
	 	exit(1);
	*/

	int server_fd = 0, child_fd;
	struct sockaddr_in  child_addr;
	unsigned short port = atoi(argv[1]);
	// pthread_t thread;
	socklen_t lensock = sizeof(child_addr);
	server_fd = Listen(server_fd, &port, argv[2]);
	printf("%d\n", getpid());

	int fdmax[FD_SETSIZE];
	fd_set FD, fd_temp;
	FD_ZERO(&FD);
	for(int i = 0; i < FD_SETSIZE; i++)
		fdmax[i] = -1;

	fdmax[0] = server_fd;
	char buf[N];
	int n, max;
	while(1)
	{
		max = FD_Max(fdmax, &FD);
		select(max + 1, &FD, NULL, NULL, NULL);
		if(FD_ISSET(server_fd, &FD))
		{
			child_fd = accept(server_fd, (AS)&child_addr, &lensock);
			printf("accept max = %d child_fd = %d\n", max, child_fd);
			insert(child_fd, fdmax);
			printf("accept2\n");
		}

		// while(1)
		for(int i = 1; i < FD_SETSIZE; i++)
		{
			if(fdmax[i] != -1 && FD_ISSET(fdmax[i], &FD))
			{
				Write(STDOUT_FILENO, "accept3\n");
				n = read(fdmax[i], buf, sizeof(buf));
				if(n > 0)
				{
					write(STDOUT_FILENO, buf, n);
					write(child_fd, buf, n);
				}
				else if(n == 0)
				{
					close(fdmax[i]);
					FD_CLR(fdmax[i], &FD);
					fdmax[i] = -1;
					printf("baibai\n");
					break;
				}
			}
		}
	}

	close(server_fd);

	exit(EXIT_SUCCESS);
}
