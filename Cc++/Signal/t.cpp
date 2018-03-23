/*************************************************************************
    > File Name: t.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月13日 星期二 12时58分45秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

void unix_error(const char *msg)
{
	fprintf(stderr, "%s : %s\n", msg, strerror(errno));

	exit(0);
}

void handler_1(int)
{
	int t = errno;
	
	if(waitpid(-1, NULL, 0) < 0)
		unix_error("waitpid error");
	write(STDOUT_FILENO, "child\n", 6);

	sleep(1);

	errno = t;
}

int main(void)
{



	exit(0);
}
