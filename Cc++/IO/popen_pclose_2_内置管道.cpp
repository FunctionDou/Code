/*************************************************************************
    > File Name: popen_pclose_1_内置管道.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月26日 星期一 22时49分06秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	char line[MAXLINE];
	FILE *fpin;

	if(argc != 2)
		err_sys("error <pathname> less");

	if((fpin = popen(argv[2], "r")) == NULL)
		err_sys("error popen");

	for( ; ;)
	{
		fputs("promt> ", stdout);
		fflush(stdout);

		if(fgets(line, MAXLINE, fpin) == NULL)
			break;
		if(fputs(line, stdout) == EOF)
			err_sys("fputs error");
	}

	if(pclose(fpin) == -1)
		err_sys("pclose error");


	exit(0);
}
