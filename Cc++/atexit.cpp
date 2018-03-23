/*************************************************************************
    > File Name: atexit.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月29日 星期一 15时23分17秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

static void Fun1();
static void Fun2();

int main(void)
{
	if(atexit(Fun1) != 0)
		err_sys("can't error Fun1");

	if(atexit(Fun2) != 0)
		err_sys("can't error Fun2");

	printf("hello world\n");

	
	exit(0);
}


void Fun1()
{
	printf("one\n");
}

void Fun2()
{
	printf("two\n");
}
