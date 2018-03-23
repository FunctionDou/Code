/*************************************************************************
    > File Name: lseek_2_空洞文件.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月26日 星期五 00时12分00秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <fcntl.h>

char buf1[] = "abcdef";
char buf2[] = "ABCDEF";

int main(void) 
{
	int fd;
	if((fd = creat("file.txt", FILE_MODE)) < 0)
		err_sys("creat error");

	if(write(fd, buf1, 10) < 0)
		err_sys("write error");

	if(lseek(fd, 1024, SEEK_SET) == -1)
		err_sys("lseek error");

	if(write(fd, buf2, 10) != 10)
		err_sys("buf2 write error");

	exit(0);
}
/*
*************** 样列  **************
od -c file.txt
0000000   a   b   c   d   e   f  \0   A   B   C  \0  \0  \0  \0  \0  \0
0000020  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0
*
0002000   A   B   C   D   E   F  \0  \0  \0  \0
0002012
*/ 
