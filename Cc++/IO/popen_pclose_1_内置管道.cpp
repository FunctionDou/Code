/*************************************************************************
    > File Name: popen_pclose_1_内置管道.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月26日 星期一 22时55分28秒

	popen 只提供连接到另一个进程的标准输入或输出的一个单项通道
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "apue.h"
#include <ctype.h>

int main(void)
{
	int c;

	while((c = getchar()) != EOF)
	{
		if(isupper(c))
			c = tolower(c);
		if(putchar(c) == EOF)
			err_sys("output error");

		if(c == '\n')
			fflush(stdout);

	}

	exit(0);
}
