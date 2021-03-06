/*************************************************************************
    > File Name: init_2_守护进程创建.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月07日 星期三 16时45分36秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <fcntl.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <signal.h>


#define LOCKFILE "/var/run/daemon.pid"
#define LOCKMODE (S_IRUSR | S_IWUSR)

int lockfile(int fd){
    struct flock fk;
    fk.l_type = F_WRLCK;
    fk.l_start = 0;
    fk.l_whence = SEEK_SET;
    fk.l_len = 0;
    return fcntl(fd,F_SETLK,&fk);
}

sigset_t mask;
int already_running(void)
{
	int fd;
	char buf[16];

	fd = open(LOCKFILE, O_RDWR | O_CREAT, LOCKMODE);
	if(fd < 0)
	{
		syslog(LOG_ERR, "open error %s : %s", LOCKFILE, strerror(errno));
		exit(1);
	}

	if(lockfile(fd) < 0)
	{
		if(errno == EACCES || errno == EAGAIN)
		{
			close(fd);
			return 1;
		}
		syslog(LOG_ERR, "can't lock %s : %s", LOCKFILE, strerror(errno));
		exit(1);
	}

	ftruncate(fd, 0);
	sprintf(buf, "%ld", (long)getpid());
	write(fd, buf, strlen(buf) + 1);

	// 截断为0
	return 0;
}

void reread(void)
{
	
}

void *thr_fn(void *)
{
	int err, signo;

	for( ; ; )
	{
		// 信号等待
		err = sigwait(&mask, &signo);
		if(err != 0)
		{
			syslog(LOG_ERR, "sigwait failed");
			exit(1);
		}

		switch(signo)
		{
			case SIGHUP:
				syslog(LOG_INFO, "Re-reading configuration file");
				reread();
				break;

			case SIGTERM:
				syslog(LOG_INFO, "got SIGTERM; exiting");
				exit(0);
			default:
				syslog(LOG_INFO, "unexpected signal %d\n", signo);
		}
	}

	return 0;
}

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
		fprintf(stderr, "more pathname error\n");
		exit(1);
	}

	int err;
	pthread_t tid;
	char *cmd;
	struct sigaction sa;

	// strrchr : 查找某个字符串中最后一次出现字符的位置
	// 有点像grep 中正则表达式的匹配功能
	if((cmd = strrchr(argv[0], '/')) == NULL)
		cmd = argv[0];
	else
		cmd++;
	
	// 将命令运行至后台, 成为守护进程
	daemonize(argv[0]);

	// 使其只有一个副本在运行
	if(already_running())
	{
		syslog(LOG_ERR, "daemon already, running");
		exit(1);
	}

	// 线程的c操作
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	// 后面代码就是为了创建一个线程, 用来等待信号
	if(sigaction(SIGHUP, &sa, NULL) < 0)
	{
		fprintf(stderr, "sigaction erro\n");
		exit(1);
	}
	sigfillset(&mask);
	if((err = pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0))
	{
		fprintf(stderr, "pthread_sigmask error : %s\n", strerror(errno));
		exit(1);
	}
	if((err = pthread_create(&tid, NULL, thr_fn, 0)) != 0)
	{
		fprintf(stderr, "pthread_create error : %s\n", strerror(errno));
		exit(1);
	}

	exit(0);
}

