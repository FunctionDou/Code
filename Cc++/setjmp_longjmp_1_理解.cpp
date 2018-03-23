/*************************************************************************
    > File Name: setjmp_longjmp.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月29日 星期一 16时09分01秒

	1. int setjmp(jmp_buf env);
		参数 env 即为保存上下文的 jmp_buf 结构体变量；
		如果直接调用该函数，返回值为 0； 若该函数从 longjmp 调用返回，返回值为非零，由 longjmp 函数提供。根据函数的返回值，我们就可以知道 setjmp 函数调用是第一次直接调用，还是由其它地方跳转过来的

	2. void longjmp(jmp_buf env, int val);
		参数 env 是由 setjmp 函数保存过的上下文.
		参数 val 表示从 longjmp 函数传递给 setjmp 函数的返回值，如果 val 值为0， setjmp 将会返回1，否则返回 val.
		longjmp 不直接返回，而是从 setjmp 函数中返回，longjmp 执行完之后，程序就像刚从 setjmp 函数返回一样

 ************************************************************************/

#include <stdio.h>
#include <setjmp.h>

jmp_buf env;

static int a = 1;

void Fun()
{
	a = 2;
	longjmp(env, a);
	printf("three\n");
}

int main(void)
{
	if(setjmp(env) == 0)
	{
		printf("one\n");
		Fun();
	}
	else
		printf("two\n");


	return 0;
}
