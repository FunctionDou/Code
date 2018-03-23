/*************************************************************************
    > File Name: connect_bin_accept_面向连接的服务器端.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月27日 星期二 17时53分01秒
 ************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <syslog.h>
#include "apue.h"

#define BUFLEN 128
#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen)
{
	int fd;
	int err = 0;

	// 创建套节字
	if((fd = socket(addr->sa_family, type, 0)) < 0)
		return -1;
	// bind 关联地址和套节字
	if(bind(fd, addr, alen) < 0)
		goto errout;
	if(type == SOCK_STREAM || type == SOCK_SEQPACKET)
		// listen 宣告愿意接受连接请求
		if(listen(fd, qlen) < 0)
			goto errout;

	return fd;

errout:
	err = errno;
	close(fd);
	errno = err;
	return -1;
}

void serve(int sockfd)
{
	int clfd;
	FILE *fp;
	char buf[BUFLEN];

	set_cloexec(sockfd);

	for( ; ; )
	{
		// accept 获得连接请求并建立连接
		if((clfd = accept(sockfd, NULL, NULL)) < 0)
		{
			syslog(LOG_ERR, "ruptimed : accept error: %s", strerror(errno));
			exit(1);
		}

		set_cloexec(clfd);
		// 采用管道进行输出
		if((fp = popen("/usr/bin/uptime", "r")) == NULL)
		{
			sprintf(buf, "error : %s\n", strerror(errno));
			// send 与read很像, 但是可以用来指定标志来改变处理传输数据的方式. send调用, 前提是必须套节字已经连接
			send(clfd, buf, strlen(buf), 0);
		}
		else
		{
			while(fgets(buf, BUFLEN, fp) != NULL)
				send(clfd, buf, strlen(buf), 0);
			pclose(fp);
		}

		close(clfd);
	}
}

int main(int argc, char *argv[])
{
	struct addrinfo *ailist, *aip, hint;
	int sockfd, err, n;
	char *host;

	if(argc != 1)
		err_quit("usage : ruptimed");
	if((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
		n = HOST_NAME_MAX;
	if((host = (char *)malloc(n)) == NULL)
		err_sys("malloc error");
	if(gethostname(host, n) < 0)
		err_sys("gethostname error");

	daemonize("ruptimed");
	memset(&hint, 0, sizeof(hint));
	hint.ai_flags = AI_CANONNAME;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;

	if((err = getaddrinfo(host, "ruptime", &hint, &ailist)) != 0)
	{
		syslog(LOG_ERR, "ruptimed : getaddrinfo error: %s", gai_strerror(err));
		exit(1);
	}

	for(aip = ailist; aip != NULL; aip = aip->ai_next)
		if((sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN)) >= 0)
		{
			serve(sockfd);
			exit(0);
		}

	exit(1);
}

