/*************************************************************************
    > File Name: pipe_1_分页存储.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月26日 星期一 21时49分07秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <unistd.h>
#include <sys/wait.h>

#define DEF_PAGER "/home/YF/Code/Cc++/IO"

int main(int argc, char *argv[])
{
	int n, fd[2];
	pid_t pid;
	char *pager, *argv0;
	char line[MAXLINE];
	FILE *fp;

	if(argc != 2)
		err_sys("error <pathname> less");

	if((fp = fopen(argv[1], "r")) == NULL)
		err_sys("error open %s", argv[1]);

	if(pipe(fd) < 0)
		err_sys("error pipe");

	if((pid = fork()) < 0)
		err_sys("error fork");
	else if(pid > 0)
	{
		close(fd[0]);

		while(fgets(line, MAXLINE, fp) != NULL)
		{
			n = strlen(line);
			if(write(fd[1], line, n) != n)
				err_sys("write error");
		}

		if(ferror(fp))
			err_sys("fgets error");

		close(fd[1]);

		if(waitpid(pid, NULL, 0) < 0)
			err_sys("waitpid error");
		exit(0);
	}
	else
	{
		close(fd[1]);

		if(fd[0] != STDIN_FILENO)
		{
			if(dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
				err_sys("dup2 error");


			close(fd[0]);
		}

		if((pager = getenv("PAGER")) == NULL)
			pager = DEF_PAGER;
		if((argv0 = strrchr(pager, '/')) != NULL)
			argv0++;
		else
			argv0 = pager;

		if(execl(pager, argv0, (char *)0) < 0)
			err_sys("execl error");
	}


	exit(0);
}
