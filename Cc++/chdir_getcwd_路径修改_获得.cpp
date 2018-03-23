/*************************************************************************
    > File Name: chdir_getcwd_路径修改_获得.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月26日 星期五 22时54分04秒

*****************************  getcwd  **********************************

	头文件：#include <unistd.h>

	定义函数：char * getcwd(char * buf, size_t size);
	
	getcwd()会将当前的工作目录绝对路径复制到参数buf 所指的内存空间，参数size 为buf 的空间大小。

	1、在调用此函数时，buf 所指的内存空间要足够大。若工作目录绝对路径的字符串长度超过参数size 大小，则返回NULL，errno 的值则为ERANGE。
	2、倘若参数buf 为NULL，getcwd()会依参数size 的大小自动配置内存(使用malloc())，如果参数size 也为0，则getcwd()会依工作目录绝对路径的字符串程度来决定所配置的内存大小，进程可以在使用完次字符串后利用free()来释放此空间。

	执行成功则将结果复制到参数buf 所指的内存空间, 或是返回自动配置的字符串指针. 失败返回NULL,错误代码存于errno.


*****************************  chdir  ***********************************

	头文件：#include <unistd.h>

	定义函数：int chdir(const char * path);

	函数说明：chdir()用来将当前的工作目录改变成以参数path 所指的目录.

	返回值执：行成功则返回0, 失败返回-1, errno 为错误代码.

 ************************************************************************/

#include <stdio.h>
#include "apue.h"

const int MAX = 20;

int main(void)
{
	char ptr[MAX];
	char *ptr1;

	ptr1 = getcwd(ptr, MAX);
	// ptr1 = getcwd(NULL, MAX);
	printf("%s\n%s\n", ptr1, ptr);

	
	chdir("/tmp");
	getcwd(ptr, MAX);
	printf("%s\n", ptr);

	exit(0);
}
