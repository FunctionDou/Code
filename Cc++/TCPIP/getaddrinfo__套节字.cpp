/*************************************************************************
    > File Name: getaddrinfo__套节字.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月27日 星期二 16时00分15秒
 ************************************************************************/

#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "apue.h"
#if defined(SOLARIS)
#include <netinet/in.h>
#endif

#if defined(BSD)
#include <sys/socket.h>
#include <netinet/in.h>
#endif

// 域, 通信特性. AF_  开头的地址族
void print_family(struct addrinfo *aip)
{
	printf(" family ");

	switch(aip->ai_family)
	{
		case AF_INET :
			printf("inet");
			break;
		case AF_INET6 :
			printf("inet6");
			break;
		case AF_UNIX :
			printf("unix");
			break;
		case AF_UNSPEC :
			printf("unspecified");
			break;
		default :
			printf("unknown");
	}
}

// 套节字类型
void print_type(struct addrinfo *aip)
{
	printf(" type ");

	switch(aip->ai_socktype)
	{
		case SOCK_STREAM :
			printf("stream");
			break;
		case SOCK_DGRAM :
			printf("datagram");
			break;
		case SOCK_SEQPACKET :
			printf("seqpacket");
			break;
		case SOCK_RAW :
			printf("raw");
			break;
		default :
			printf("unknown (%d)", aip->ai_socktype);
	}
}

// 传输协议
void print_protocol(struct addrinfo *aip)
{
	printf(" protocol ");

	switch(aip->ai_protocol)
	{
		case 0 :
			printf("default");
			break;
		case IPPROTO_TCP :
			printf("TCP");
			break;
		case IPPROTO_UDP :
			printf("UDP");
			break;
		case IPPROTO_RAW :
			printf("RAW");
			break;
		default :
			printf("unknown (%d)", aip->ai_protocol);
	}
}

// 自定义行为
void print_flags(struct addrinfo *aip)
{
	printf("flags");

	if(aip->ai_flags == 0)
		printf("0");
	else
	{
		if(aip->ai_flags & AI_PASSIVE)
			printf(" passive");
		if(aip->ai_flags & AI_CANONNAME)
			printf(" canon");
		if(aip->ai_flags & AI_NUMERICHOST)
			printf(" numhost");
		if(aip->ai_flags & AI_NUMERICSERV)
			printf(" numserv");
		if(aip->ai_flags & AI_V4MAPPED)
			printf(" v4mapped");
		if(aip->ai_flags & AI_ALL)
			printf(" all");
	}
}

int main(int argc, char *argv[])
{
	struct addrinfo *ailist, *aip, hint;
	struct sockaddr_in *sinp;
	const char *addr;
	int err;
	char abuf[INET_ADDRSTRLEN];

	if(argc != 3)
		err_sys("error! <nodename> <nodename> less");

	// addrinfo hint的初始化. 用于过滤地址的模板
	hint.ai_flags = AI_CANONNAME;
	hint.ai_family = 0;
	hint.ai_socktype = 0;
	hint.ai_socktype = 0;
	hint.ai_addrlen = 0;
	hint.ai_canonname = NULL;
	hint.ai_next = NULL;
	hint.ai_addr = NULL;

	// 返回ailist 的链表结构
	if((err = getaddrinfo(argv[1], argv[2], &hint, &ailist)) != 0)
		err_quit("getaddrinfo error : %s\n", gai_strerror(err));	// gai_strerror() 返回getaddrinfo 的错误信息

	for(aip = ailist; aip != NULL; aip = aip->ai_next)
	{
		print_flags(aip);
		print_family(aip);
		print_type(aip);
		print_protocol(aip);
		printf("\n\thost %s", aip->ai_canonname ? aip->ai_canonname : "-");

		if(aip->ai_family == AF_INET)
		{
			// sockaddr_in IPV4中的通用的地址结构
			sinp = (struct sockaddr_in *)aip->ai_addr;
			// 二进制转为字符串格式
			addr = inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);

			printf(" address %s", addr ? addr : "unknown");
			// 主机字节序表示的16位整数
			printf(" port %d", ntohs(sinp->sin_port));
		}

		printf("\n");
	}

	exit(0);
}
