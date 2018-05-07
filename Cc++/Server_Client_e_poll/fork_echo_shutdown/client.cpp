/*************************************************************************
    > File Name: child.cpp
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年04月24日 星期二 13时14分07秒
 ************************************************************************/
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#define AS struct sockaddr *
#define N 64

void SIG_CHLD(int);

void SIG_CHLD(int signo)
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
		printf("SIGPIPE\n");
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	struct sigaction action1, action2;
	action1.sa_handler = SIG_CHLD;
	action2.sa_handler = SIG_CHLD;
	if(sigaction(SIGCHLD, &action1, NULL) == -1 && sigaction(SIGPIPE, &action2, NULL) == -1)
		exit(1);

	int sock_fd;
	unsigned short port;
	struct sockaddr_in child_addr;
	port = atoi(argv[1]);
	socklen_t len_sock;
	len_sock = sizeof(child_addr);

	bzero(&child_addr, len_sock)	;
	child_addr.sin_family = AF_INET;
	child_addr.sin_port = htons(port);
	// child_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	child_addr.sin_addr.s_addr = inet_addr(argv[2]);
	// sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	sock_fd  =socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(connect(sock_fd, (struct sockaddr *)&child_addr, len_sock) == -1)
	{
		perror("connect error : ");
		exit(1);
	}
	
	printf("success\n");
	char buf[N];
	int n;
	
	fd_set FD, fd_temp;
	FD_ZERO(&FD);
	FD_SET(STDIN_FILENO, &FD);
	FD_SET(sock_fd, &FD);
	
	int max;
	max = STDIN_FILENO > sock_fd ? STDIN_FILENO + 1 : sock_fd + 1;
	while(1)
	{
		
		fd_temp = FD;
		// select(max, NULL, &fd_temp, NULL, NULL);
		select(max, &fd_temp, NULL, NULL, NULL);
		if(FD_ISSET(STDIN_FILENO, &fd_temp))
		{
			n = read(STDIN_FILENO, buf, sizeof(buf));
			if(n > 0)
				write(sock_fd, buf, n);
			else if(n == 0)
			{
				// ++
				// 不直接 break，而是先执行半关闭操作，然后继续执行循环. 等待服务器将数据发送过来完之后, 等待服务器关闭之后在关闭
				// printf("duang\n");
				shutdown(sock_fd, SHUT_WR);
				FD_CLR(sock_fd, &FD);
				
				// break;
			}
		}
		else if(FD_ISSET(sock_fd, &fd_temp))
		{
			n = read(sock_fd, buf, sizeof(buf));
			if(n > 0)
				write(STDOUT_FILENO, buf, n);
			else if(n == 0)
			{
				printf("duang\n");
				exit(0);
			}
		}
	}


	exit(EXIT_SUCCESS);
}
