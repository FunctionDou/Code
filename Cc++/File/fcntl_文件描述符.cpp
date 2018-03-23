/*************************************************************************
    > File Name: fcntl_文件描述符.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月26日 星期五 20时49分07秒


	fcntl ->  功能：操纵文件描述符，改变已打开的文件的属性

	int fcntl(int fd, int cmd, ... );

	cmd的取值可以如下：

	复制文件描述符
	F_DUPFD (long)

	设置/获取文件描述符标志
	F_GETFD (void)
	F_SETFD (long)

	设置/获取文件状态标志
	F_GETFL (void)
	F_SETFL (long)

	获取/设置文件锁
	F_GETLK
	F_SETLK，F_SETLKW

 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <fcntl.h>

int main(int argc, char * argv[])
{
	int val;

	if(argc != 2)
		err_quit("usage: a.out <descriptor");

	if((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0)
		err_sys("fcntl error for fd %d", atoi(argv[1]));

	switch (val & O_ACCMODE)
	{
		case O_RDONLY:
			printf("read only");
			break;
		case O_WRONLY:
			printf("write only");
			break;
		case O_RDWR:
			printf("read write");
			break;

		default:
			err_dump("unknown access mode");
	}

	if(val & O_APPEND)
		printf(", append");

	if(val & O_NONBLOCK)
		printf(", nonblocking");

	if(val & O_SYNC)
		printf(", synchronous write");

#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
	if(val & O_FSYNC)
		printf(", synchronous writes");

#endif
	putchar('\n');


	exit(0);
}

