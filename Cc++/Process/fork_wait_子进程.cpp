/*************************************************************************
    > File Name: fork_wait_子进程.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月30日 星期二 11时23分27秒

	
	pid_t waitpid(pid_t pid, int *statloc, int options)
	
	可以并不堵塞
	可以等待pid指定的进程终止在执行waitpid()的调用者。
	statloc ！= NULL 则进程的终止状态就存放在其所指的单元， statloc == NULL 则不关心其终止状态
	
	pid的值
	
	pid == -1 等待任意子进程
	pid > 0 等待 “进程ID == pid” 的进程
	pid == 0 等待 ”组ID == 调用进程组ID”的任意进程
	pid < -1 等待 “组ID == |pid|“ 的进程

 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <sys/wait.h>

int main(void)
{
	char buf[] = "fork\n";
	char buf1[] = "fork pid > 0\n";
	char buf2[] = "fork fork\n";

	// pid_t pid = -1;
	pid_t pid;
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
	{
		if(write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1)
			err_sys("write buf error");

		if((pid = fork()) < 0)
			err_sys("fork error");
		else if(pid > 0)
		{
			if(write(STDOUT_FILENO, buf1, sizeof(buf1) - 1) != sizeof(buf1) - 1)
				err_sys("write buf1 error");

			exit(0);
		}

		if(write(STDOUT_FILENO, buf2, sizeof(buf2) - 1) != sizeof(buf2) - 1)
			err_sys("write buf2 error");

		sleep(2);
		printf("second child, parent pid = %ld\n", (long)getppid());
		exit(0);
	}

	if(waitpid(pid, NULL, 0) != pid)
		err_sys("waitpid error");

	exit(0);
}
/*          样例
[root@192 Process]# ./a.out 
fork
fork pid > 0
fork fork
[root@192 Process]# second child, parent pid = 1

[root@192 Process]# 
*/
