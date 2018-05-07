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

#define AS struct sockaddr *
#define N 64

int Listen(int , unsigned short *);
void * child_thread(void *);

void * child_thread(void *fd)
{
	int FD = *(int *)fd;
	char buf[N];
	int n;
	n = read(FD, buf, sizeof(buf));
	write(STDOUT_FILENO, buf, n);

	pthread_exit((void *)0);
}


int Listen(int fd, unsigned short * port)
{
	struct sockaddr_in server_addr;
	socklen_t len_address;
	
	len_address = sizeof(server_addr);
	bzero(&server_addr, len_address);
	// fd = socket(AF_INET, SOCK_STREAM, 0);
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	server_addr.sin_family = AF_INET;
	// server_addr.sin_port = htons(*port);
	server_addr.sin_addr.s_addr = *port;
	// server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(bind(fd, (AS)&server_addr, len_address) < 0)
		exit(1);

	if(listen(fd, 5) < 0)
		exit(1);

	return fd;
}

int main(int argc, char *argv[])
{
	int server_fd = 0, child_fd;
	struct sockaddr_in  child_addr;
	unsigned short port = 9734;
	pthread_t thread;
	socklen_t lensock = sizeof(child_addr);
	server_fd = Listen(server_fd, &port);
	
	char buf[N];
	int n;
	while(1)
	{
		child_fd = accept(server_fd, (AS)&child_addr, &lensock);
		printf("server\n");
		// pthread_create(&thread, NULL, child_thread, (void *)&child_fd);
		//close(child_fd);
		n = read(child_fd, buf, sizeof(buf));
		write(STDOUT_FILENO, buf, n);
	}
	close(server_fd);

	exit(EXIT_SUCCESS);
}
