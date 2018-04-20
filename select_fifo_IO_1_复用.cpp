/*
int select(int nfds, fd_set *readnfds, fd_set *writenfds, fd_set *exceptfds, struct timeval *timeout);

// nfds 是最大文件描述符 + 1
nfds = max(fd) + 1;

fd_set 文件读/写的集合

// timeout : 时间设置

返回值:
	返回值 < 0 : 函数执行错误
	返回值 = 0 : 超时
	返回值 > 0 : 监听描述符集中有几个时间安发生
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/select.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>

#define N 64

void write_error(const char *);
int Print(int);
int create_fifo(const char *);
void  sig_sigint(int);

void sig_sigint(int)
{
	unlink("a.fifo");
	unlink("b.fifo");
}

int create_fifo(const char *s1)
{
	int fifo;
	fifo = mkfifo(s1, 0644);

	if(!fifo)
		return -1;
	return 0;
}

int Print(int fd)
{
	int n;
	char buf[N], line[N];
	n = read(fd, buf, sizeof(buf));
	if(n > 0)
	{
		sprintf(line, "fd[%d] : %s\n", fd, buf);
		puts(line);
	}
	else if(n < 0)
		write_error("read error");
	else if(n == 0)
	{
		sprintf(line, "%d close\n", fd);
		puts(line);
	}

	return n;
}


void write_error(const char *s)
{
	write(STDERR_FILENO, s, strlen(s));

	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	int fd[2];
	fd_set FD;

	struct sigaction action;
	// sigemptyset(&action.sa_mask);
	// sigaddset(&action.sa_mask, SIGINT);
	action.sa_handler = sig_sigint;
	sigaction(SIGINT, &action, NULL);

	if(create_fifo("a.fifo") == -1 && create_fifo("b.fifo") == -1)
		write_error("create mkfifo error\n");

	fd[0] = open("a.fifo", O_RDONLY);
	fd[1] = open("b.fifo", O_RDONLY);
	FD_SET(fd[0], &FD);
	FD_SET(fd[1], &FD);

	int max, n;
	max = fd[1] + 1;

	fd_set FD_tmp;
	while(1)
	{
		FD_tmp = FD;
		if(select(max, &FD_tmp, NULL, NULL, NULL) < 0)
			write_error("select error");

		if(FD_ISSET(fd[0], &FD_tmp))
		{
			n = Print(fd[0]);
			if(n == 0)
				FD_CLR(fd[0], &FD);
		}
		if(FD_ISSET(fd[1], &FD_tmp))
		{
			n = Print(fd[1]);
			if(n == 0)
				FD_CLR(fd[1], &FD);

		}
	}
	
	exit(EXIT_SUCCESS);
}
