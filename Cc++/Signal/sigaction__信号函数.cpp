/*************************************************************************
    > File Name: t.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月05日 星期一 16时10分38秒

*******************************  sigaction ******************************

	#include <signal.h>

	 struct sigaction
	 {
	    void     (*sa_handler)(int);					    // 要执行信号的函数指针
	    void     (*sa_sigaction)(int, siginfo_t *, void *); // 信号处理函数
	    sigset_t  sa_mask;									// 屏蔽的信号集
	    int       sa_flags;									// 指定信号的行为
	    void     (*sa_restorer)(void);
	 };

	// 检查或修改与指定信号相关联的处理动作
	int sigaction(int signum, struct sigaction *atc, struct sigaction *oldatc)
	// 返回值, 成功 0; 失败, -1.
	

	1. sigaction 函数
		 ◆ signum：要操作的信号。可以指定SIGKILL 和SIGSTOP 以外的所有信号。
		 ◆ act：要设置的对信号的新处理方式。
		 ◆ oldact：原来对信号的处理方式。

	2. sigaction 结构体
	   sa_flags : 指定信号行为

		 ◆ SA_RESTART：使被信号打断的系统调用自动重新发起。
		 ◆ SA_NOCLDSTOP：使父进程在它的子进程暂停或继续运行时不会收到 SIGCHLD 信号。
		 ◆ SA_NOCLDWAIT：使父进程在它的子进程退出时不会收到 SIGCHLD  信号，这时子进程如果退出也不会成为僵尸进程。
		 ◆ SA_NODEFER：使对信号的屏蔽无效，即在信号处理函数执行期间仍能发出这个信号。
		 ◆ SA_RESETHAND：信号处理之后重新设置为默认的处理方式。
		 ◆ SA_SIGINFO：使用 sa_sigaction 成员而不是 sa_handler 作为信号处理函数。

 ************************************************************************/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>

static jmp_buf jmp;

static void sig_int(int signo)
{
	if(signo == SIGINT)
		printf("SIGINT\n");
	if(signo == SIGALRM)
		printf("SIGALRM\n");
	if(signo == SIGABRT)
		printf("SIGABRT\n");

	longjmp(jmp, 1);
}

int main(void)
{
	struct sigaction newatc, oldatc;
	sigset_t sigset;
	
	sigemptyset(&sigset);
	sigfillset(&sigset);
	sigaddset(&sigset, SIGINT);
	sigaddset(&sigset, SIGALRM);

	newatc.sa_mask = sigset;
	newatc.sa_handler = sig_int;

	// sigaction 类似于signal 函数一样
	if(sigaction(SIGINT, &newatc, &oldatc) != 0)
		fprintf(stderr, "sigaction(SIGINT) error");

	if(setjmp(jmp) == 0)
		pause();

	exit(0);
}
