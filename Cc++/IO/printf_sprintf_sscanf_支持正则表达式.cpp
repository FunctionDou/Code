/*************************************************************************
    > File Name: printf_sprintf_sscanf_支持正则表达式.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月26日 星期一 23时23分24秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int main(void)
{
	int n, num1, num2;
	char line[MAXLINE];

	while((n = read(STDIN_FILENO, line, MAXLINE)) > 0)
	{
		// NULL 结尾
		line[n] = 0;
		
		if(sscanf(line, "%d%d", &num1, &num2) == 2)
		{
			sprintf(line, "%d\n", num1 + num2);
			n = strlen(line);
			if(write(STDOUT_FILENO, line, n) != n)
				err_sys("error write");
		}
		else
			if(write(STDOUT_FILENO, "error sscanf", 12) != 12)
				err_sys("write error sscanf");
	}

	exit(0);
}
