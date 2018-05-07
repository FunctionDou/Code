/*************************************************************************
    > File Name: server_select_e_poll.cpp
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年04月27日 星期五 08时52分54秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	struct sockaddr_in addr1, addr2; 
	socklen_t len;
	int port;

	port = atoi(argv[1]);
	addr1.sin_port = htons(port);
	addr1.sin_family = AF_INET;
	addr1.sin_addr.s_addr = inet_addr(argv[2]);
	len = sizeof(addr1);

	int sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	bind(sock, (struct sockaddr *)&addr1, len);
	listen(sock, 5);
	
	int childfd;
	
	while(1)
	{
		childfd = accept(sock, (struct sockaddr *)&addr2, &len);
		pid_t pid;
		if((pid = fork()) > 0)
		{
			
		}
		else if(pid < 0)
			exit(1);
	}

	exit(0);
}
