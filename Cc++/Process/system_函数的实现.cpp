/*************************************************************************
    > File Name: system_函数的实现.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月01日 星期四 22时43分18秒
 ************************************************************************/

#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include "apue.h"

int system(const char *cmdstring)
{
	pid_t pid;
	int status;

	if(cmdstring == NULL)
		return 1;

	if((pid = fork()) < 0) 
		status = -1;
	else if(pid == 0)
	{
		// shell 的-c选项是告诉shell程序读取下一个命令行参数(这里是cmdstring)作为命令输入(而不是从标准输入或者指定文件读命令);
		execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
		_exit(0);
	}
	else
	  while(waitpid(pid, &status, 0) < 0)
			if(errno != EINTR)
			{
				status = -1;
				break;
			}

	return status;
}

void
pr_exit(int status)
{
  // 正常终止
  if (WIFEXITED(status))
    printf("normal termination, exit status = %d\n",
        WEXITSTATUS(status));
  // 异常终止
  else if (WIFSIGNALED(status))
    printf("abnormal termination, signal number = %d%s\n",
        WTERMSIG(status),
#ifdef WCOREDUMP
        WCOREDUMP(status) ? " (core file generated)" : ""
#else
        ""
#endif
        );
  // 暂停子进程
  else if (WIFSTOPPED(status))
    printf("child stopped, signal number = %d\n", WSTOPSIG(status));
}

int main(void)
{
	int status;

	if((status = system("date")) < 0)
		err_sys("system() error");

	pr_exit(status);

	if((status = system("no file")) < 0)
		err_sys("system() error");

	pr_exit(status);

	if((status = system("who; exit 44")) < 0)
		err_sys("system() error");

	pr_exit(status);


	exit(0);
}

/*     样列
 *
 [root@192 Process]# ./a.out 
2018年 02月 01日 星期四 22:57:00 CST
normal termination, exit status = 0
sh: no: 未找到命令
normal termination, exit status = 127
root     :2           2018-02-01 14:30 (:2)
root     pts/0        2018-02-01 20:58 (:2)
root     pts/1        2018-02-01 21:40 (:2)
root     pts/2        2018-02-01 21:56 (:2)
normal termination, exit status = 44

*/

