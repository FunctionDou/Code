/*************************************************************************
    > File Name: init_守护进程创建.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月07日 星期三 13时11分24秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>

void daemonize(const char *cmd)
{
	int fd0, fd1, fd2;
	pid_t pid;
	struct rlimit rl;
	struct sigaction atc;

	umask(0);

	if(getrlimit(RLIMIT_NOFILE, &rl) < 0)
	{
		fprintf(stderr, "getrlimit error");
		exit(1);
	}
	
	// 新建进程
	if((pid = fork()) < 0)
	{
		fprintf(stderr, "fork error");
		exit(1);
	}
	// 父进程退出, 只留子进程
	else if(pid != 0)
		exit(0);
	// 将新近程设置为新会话的组长
	setsid();

	// sigaction 结构体
	atc.sa_handler = SIG_IGN;
	// 设置好信号集
	sigemptyset(&atc.sa_mask);
	atc.sa_flags = 0;
	// 信号处理
	if(sigaction(SIGHUP, &atc, NULL) < 0)
	{
		fprintf(stderr, "sigaction error");
		exit(1);
	}

	// 再新建一个进程, 打开子进程, 在关闭前一个子进程.
	// 目地是为了禁止进程重新打开一个控制终端, 使其真正的称为守护进程
	if((pid = fork()) < 0)
	{
		fprintf(stderr, "fork() error");
		exit(1);
	}
	else if(pid > 0)
		exit(0);

	// getcwd 获取当前目录
	// 修改当前进程的目录是根目录
	if(chdir("/") < 0)
	{
		fprintf(stderr, "chdir error");
		exit(1);
	}

	// 关闭所有打开的文件描述符
	if(rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for(rlim_t i = 0; i < rl.rlim_max; i++)
		close(i);

	// 最后将所有的信息输出到 NULL 里.
	fd0 = open("dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	// openlog， syslog， closelog是一套系统日志写入接口。
	// openlog 函数用来打开一个到系统日志记录程序的连接，打开之后就可以用syslog或vsyslog函数向系统日志里添加信息了。而closelog函数就是用来关闭此连接的。
	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if(fd0 != 0 || fd1 != 1 || fd2 != 2)
	{
		syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
		exit(1);
	}
}

int main(int argc, char * argv[])
{
	if(argc != 1)
	{
		fprintf(stderr, "more pathname error");
		exit(1);
	}

	daemonize(argv[0]);

	exit(0);
}

/*			样例

[root@localhost Process]# ps -efj | grep ./a.out 
root     12288  8593 12287  8593  0 16:03 pts/1    00:00:00 grep --color=auto ./a.out

*/
