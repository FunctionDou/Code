/*************************************************************************
    > File Name: pipe__管道.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月26日 星期一 18时35分04秒

	管道只能用于两个进程之间的线性连接.

	FIFO是有名字的, 可以进行非线性的连接
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <unistd.h>

int main(void)
{
	int n;
	int fd[2];
	pid_t pid;
	char line[MAXLINE];

	if(pipe(fd) < 0)
		err_sys("pipe error");

	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid > 0)
	{
		close(fd[0]);
		write(fd[1], "hello world\n", 12);
	}
	else
	{
		close(fd[1]);
		n = read(fd[0], line, MAXLINE);
		write(STDOUT_FILENO, line, n);
	}


	exit(0);
}
