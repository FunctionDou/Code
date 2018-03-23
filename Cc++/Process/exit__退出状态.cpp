/*************************************************************************
    > File Name: exit__退出状态.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月31日 星期三 21时40分13秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <sys/wait.h>

void pr_exit(int status)
{
	// 正常终止
	if(WIFEXITED(status))
		printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
	// 异常终止
	else if(WIFSIGNALED(status))
		printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status), 
#ifdef WCOREDUMP
		WCOREDUMP(status) ? " (core file generated)" : "");
#else
		"");
#endif
	// 暂停终止
	else if(WIFSTOPPED(status))
		printf("child stopped, signal number = %d", WSTOPSIG(status));

}


int main(void)
{
	pid_t pid;
	int status;

	// 1
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
		exit(7);

	if(wait(&status) != pid)
		err_sys("wait error");
	pr_exit(status);

	// 2
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
		abort();
	
	if(wait(&status) != pid)
		err_sys("wait error");
	pr_exit(status);


	// 3
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
		status /= 0;

	if(wait(&status) != pid)
		err_sys("wait error");
	pr_exit(status);


	exit(0);
}

/*      样例
 ./a.out 
normal termination, exit status = 7
abnormal termination, signal number = 6 (core file generated)
abnormal termination, signal number = 8 (core file generated)
*/ 
