/*************************************************************************
    > File Name: strerror_perror.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月25日 星期四 23时22分29秒

************************  strerror()   *****************************
	头文件：#include <string.h>

	定义函数：char * strerror(int errnum);
	
	函数说明：strerror()用来依参数errnum 的错误代码来查询其错误原因的描述字符串, 然后将该字符串指针返回.

	返回值：返回描述错误原因的字符串指针.


************************* perror  *********************************

	头文件：#include <stdio.h>

	函数perror()用于抛出最近的一次系统错误信息

    函数定义 : void perror(char *string);

	string为要输出的错误信息
	perror()用来将上一个函数发生错误的原因输出到标准错误(stderr)。
	参数string所指的字符串会先打印出，后面再加上错误原因字符串，此错误原因依照全局变量errno 的值来决定要输出的字符串。

 ************************************************************************/

#include <stdio.h>
#include <errno.h>
#include "apue.h"
#include <string.h>

int main(int argc, char * argv[])
{
	fprintf(stderr, "EACCES: %s\n", strerror(EACCES));
	errno = ENOENT;
	perror(argv[0]);

	errno = 1;
	perror("error : ");
	

	int i;
	for (i = 0; i < 10; i++)
		printf("%d error : %s\n", i, strerror(i));

	exit(0);
}
