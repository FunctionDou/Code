/*************************************************************************
    > File Name: connect_客户端.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月09日 星期五 09时18分12秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>

int main(void)
{
	int sockfd, len;
	struct sockaddr_in address;
	int result;
	// char ch = 'A';

	// 设置网络设置
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	// 设置端口号
	address.sin_port = 9734;

	// 请求连接
	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *) &address, len);
	if(result == -1)
	{
		perror("connect error");
		return -1;
	}

	char ch;
	while(1)
	{
		read(STDIN_FILENO, &ch, 1);
		
		if(write(sockfd, &ch, 1) != 1)
		{
			fprintf(stderr, "write error\n");
			exit(1);
		}
		/*
		read(sockfd, s, n);
		printf("come server : %s\n", s);
		*/
		// sleep(1);
	}

	close(sockfd);

	exit(0);
}
