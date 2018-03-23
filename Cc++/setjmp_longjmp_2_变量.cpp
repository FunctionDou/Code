/*************************************************************************
    > File Name: setjmp_longjmp_2_变量.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月29日 星期一 16时31分41秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <setjmp.h>

static void Fun1(int, int, int, int, int);
static void Fun2();

static jmp_buf jmpbuffer;
static int oneval = 1;

int main(void)
{
	static int twoval = 2;
	volatile int threeval = 3;
	int autoval = 4;
	register int regival = 5;

	printf("first\n");
	printf("oneval = %d twoval = %d threeval = %d\n autoval = %d regival = %d\n\n", oneval, twoval, threeval, autoval, regival);
	
	if(setjmp(jmpbuffer) != 0)
	{
		printf("after\n");

		printf("oneval = %d twoval = %d threeval = %d\n autoval = %d regival = %d\n", oneval, twoval, threeval, autoval, regival);
		exit(0);
	}
	
	oneval = 10; twoval = 20; threeval = 30; autoval = 40; regival = 50;
	Fun1(oneval, twoval, threeval, autoval, regival);

	exit(0);
}

static void Fun1(int i, int j, int k, int m, int t)
{
	Fun2();
}

static void Fun2()
{
	longjmp(jmpbuffer, 1);
}
