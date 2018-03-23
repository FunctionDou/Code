/*************************************************************************
    > File Name: gethost_information_查询主机信息.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月10日 星期六 09时55分44秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define Max 128

int main(int argc, char *argv[])
{
	char *host;					// 保存主机名
	char buf[Max];
	int sockfd;					// 保存套接字标识符
	int len;
	struct sockaddr_in address;	// 第一套接字地址
	struct hostent *hostinfo;	// 主机信息结构
	struct servent *servinfo;	// 服务器信息结构

	if(argc == 1)
		host = "localhost";
	else
		host = argv[1];
	if((hostinfo = gethostbyname(host)) == 0)
	{
		fprintf(stderr, "gethostbyname error");
		exit(1);
	}
	if((servinfo = getservbyname("daytime", "tcp")) == 0)		// 获得服务信息
	{
		fprintf(stderr, "getservbyname error");
		exit(1);
	}
	printf("daytime port is : %d\n", ntohs(servinfo->s_port));
	
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	address.sin_family = AF_INET;
	address.sin_port = servinfo->s_port;
	address.sin_addr = *(struct in_addr *) *hostinfo->h_addr_list;
	
	len = sizeof(address);
	if((connect(sockfd, (struct sockaddr *)&address, len)) != 0)
	{
		perror("connect error");
		exit(1);
	}

	int n;
	n = read(sockfd, &buf, sizeof(buf));
	buf[n] = '\0';
	printf("buf is : %s\n", buf);

	close(sockfd);

	exit(0);
}
