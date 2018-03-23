/*************************************************************************
    > File Name: socket_bind_服务器端.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月09日 星期五 09时02分21秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void)
{
	int server_sockfd, client_sockfd;
	int  server_len, client_len;
	struct sockaddr_in server_address, client_address;
	
	// server_address 设置信息
	server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	// 设置端口号
	server_address.sin_port = 9734;
	server_len = sizeof(server_address);

	// 主机与地址进行关联
	if(bind(server_sockfd, (struct sockaddr *)&server_address, server_len) != 0)
	{
		fprintf(stderr, "bind error");
		exit(1);
	}

	// 进行网络监听
	listen(server_sockfd, 5);

	// 接受请求
	client_len = sizeof(client_address);
	client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, (socklen_t * __restrict) &client_len);
	

	// 写读
	while(1)
	{
		char ch;

		/*
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, (socklen_t * __restrict) &client_len);
		*/

		if(read(client_sockfd, &ch, 1) <= 0)
			exit(1);
		if(write(STDOUT_FILENO, &ch, 1) != 1)
		{
			fprintf(stderr, "write error");
			exit(1);
		}
		// sleep(1);

		// close(client_sockfd);
	}

	close(client_sockfd);
	exit(0);
}

