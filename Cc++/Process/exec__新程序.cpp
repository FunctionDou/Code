/*************************************************************************
    > File Name: exec__新程序.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月01日 星期四 21时48分25秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <sys/wait.h>

char *env_init[] = {"USER = unknown", "PATH=/tmp", NULL};

int main(void)
{
	pid_t pid;
	
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
		if(execle("/bash/sh", "echoll", "myarg1", "MY ARG2", (char *) 0, env_init) < 0)
			err_sys("execle error");

	if(waitpid(pid, NULL, 0) < 0)
		err_sys("waitpid error");
	else if(pid == 0)
		if(execlp("echoll", "echoll", "only 1 arg", (char *)0) < 0)
			err_sys("execlp error");


	exit(0);
}


