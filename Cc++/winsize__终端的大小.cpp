/*************************************************************************
    > File Name: winsize__终端的大小.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月04日 星期日 11时04分11秒
 ************************************************************************/

#include <stdio.h>
#include <termios.h>
#include "apue.h"

#ifdef TIOCGWINSZ
#include <sys/ioctl.h>
#endif

static void pr_winsize(int fd)
{
	struct winsize size;

	if(ioctl(fd, TIOCGWINSZ, (char *) &size) < 0)
		err_sys("TIOCGWINSZ error");
	printf("%d rows, %d colums\n", size.ws_row, size.ws_col);
}

static void sig_winch(int signo)
{
	printf("SIGWINCH received\n");
	pr_winsize(STDIN_FILENO);
}

int main(void)
{
	if(isatty(STDIN_FILENO) == 0)
		exit(1);
	if(signal(SIGWINCH, sig_winch) == SIG_ERR)
		err_sys("signal error");
	
	pr_winsize(STDIN_FILENO);
	for( ; ; )
		pause();
}

