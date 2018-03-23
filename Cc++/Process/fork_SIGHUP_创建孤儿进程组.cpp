/*************************************************************************
    > File Name: fork_SIGHUP_创建孤儿进程组.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月02日 星期五 14时50分45秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <errno.h>

static void sig_hup(int signo)
{
	printf("SIGHUP received, pid = %ld\n", (long)getpid());
}

static void pr_ids(char *name)
{
	printf("%s : pid = %ld, ppid = %ld, pgrp = %ld, tpgrp = %ld\n", name, (long)getpid(), (long)getppid(), (long)getpgrp(), (long)tcgetpgrp(STDIN_FILENO));

	fflush(stdout);
}

int main(void)
{
	char c;
	pid_t pid;

	pr_ids("paren");
	if((pid = fork()) < 0)
		err_sys("error fork");
	else if(pid > 0)
		sleep(2);
	else
	{
		pr_ids("child");
		signal(SIGHUP, sig_hup);
		kill(getpid(), SIGTSTP);

		pr_ids("child");
		if(read(STDIN_FILENO, &c, 1) != 1)
			err_sys("read error %d on controlling TTY\n", errno);
	}

}

