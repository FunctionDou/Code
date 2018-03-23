/*************************************************************************
    > File Name: readn_writen_读写指定字节.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月26日 星期一 16时25分35秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

ssize_t readn(int fd, void *ptr, size_t n)
{
	size_t nleft;
	size_t nread;
	nleft = n;

	while(nleft > 0)
	{
		if((nread = read(fd, ptr, nleft)) < 0)
			if(nleft == n)
				return -1;
			else
				break;
		else if(nread == 0)
			break;
		 
		nleft -= nread;
		ptr += nread;
	}

	return (n - nleft);
}

ssize_t writen(int fd, void *ptr, size_t n)
{
	size_t nleft;
	size_t nwrite;
	nleft = n;

	while(nleft > 0)
	{
		if((nwrite = write(fd, ptr, nleft)) < 0)
			if(nleft == n)
				return -1;
			else
				break;
		else if(nwrite == 0)
			break;

		nleft -= nwrite;
		ptr += nwrite;
	}

	return (n - nleft);
}
