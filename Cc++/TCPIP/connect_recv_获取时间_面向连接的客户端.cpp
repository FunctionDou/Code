/*************************************************************************
    > File Name: connect_recv_服务器连接获取时间.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月27日 星期二 17时28分07秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include "apue.h"

#define MAXSLEEP 128
#define BUFLEN 128

int connect_retry(int domain, int type, int protocol, const struct sockaddr *addr, socklen_t alen)
{
	int numsec, fd;

	for(numsec = 1; numsec <= MAXSLEEP; numsec <<= 1)
	{
		// 创建套节字, domain 域; type 套节字类型; protocol 传输协议
		if((fd = socket(domain, type, protocol)) < 0)
			return -1;

		// 建立连接
		if(connect(fd, addr, alen) == 0)
			return fd;
		close(fd);

		if(numsec <= MAXSLEEP / 2)
			sleep(numsec);
	}

	return -1;
}

// sockfd 套节字描述符
void print_uptime(int sockfd)
{
	int n;
	char buf[BUFLEN];

	// 从服务器读
	while((n = recv(sockfd, buf, BUFLEN, 0)) > 0)
		write(STDOUT_FILENO, buf, n);
	if(n < 0)
		err_sys("recv error");
}

int main(int argc, char *argv[])
{
	struct addrinfo *ailist, *aip;
	struct addrinfo hint;
	int sockfd, err;

	if(argc != 2)
		err_quit("usage: ruptime hostname");

	memset(&hint, 0, sizeof(hint));
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;

	if((err = getaddrinfo(argv[1], "ruptime", &hint, &ailist)) != 0)
	{
		// 建立连接, 返回套节字描述符
		if((sockfd = connect_retry(aip->ai_family, SOCK_STREAM, 0, aip->ai_addr, aip->ai_addrlen)) < 0)
			err = errno;
		else
		{
			print_uptime(sockfd);
			exit(0);
		}
	}

	err_exit(err, "can't connect to %s", argv[1]);
}

