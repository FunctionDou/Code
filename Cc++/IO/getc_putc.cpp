/*************************************************************************
    > File Name: getc_putc.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月25日 星期四 23时17分27秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int main(void)
{
	int tmp;
	while((tmp = getc(stdin)) != EOF)
		if(putc(tmp, stdout) == EOF)
			printf("output error");

	if(ferror(stdin) < 0)
		err_sys("Input error");

	return 0;
}
