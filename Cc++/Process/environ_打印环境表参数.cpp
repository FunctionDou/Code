/*************************************************************************
    > File Name: environ_打印环境表参数.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月01日 星期四 21时58分25秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int main(int argc, char *argv[])
{
	int i;
	char **ptr;
	extern char **environ;

	for(i = 0; i < argc; i++)
		printf("argv[%d] : %s\n", i, environ[i]);

	for(ptr = environ; *ptr != 0; ptr++)
		printf("%s\n", *ptr);


	exit(0);
}
