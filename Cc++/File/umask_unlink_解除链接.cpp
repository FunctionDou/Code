/*************************************************************************
    > File Name: umask_unlink_解除链接.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月26日 星期五 22时09分16秒

	相关函数：link, rename, remove

	头文件：#include <unistd.h>

	定义函数：int unlink(const char * pathname);

	函数说明：unlink()会删除参数pathname 指定的文件. 如果该文件名为最后连接点, 但有其他进程打开了此文件, 则在所有关于此文件的文件描述词皆关闭后才会删除. 如果参数pathname 为一符号连接, 则此连接会被删除。

	返回值：成功则返回0, 失败返回-1, 错误原因存于errno

 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <fcntl.h>

int main(void)
{
	if(open("one", O_RDWR) < 0)
		err_sys("open one error");
	if(unlink("one") < 0)
		err_sys("unlink one error");
	printf("file one unlink \n");

	if(open("two", O_RDWR) < 0)
		err_sys("open two error");
	if(unlink("two") < 0)
		err_sys("unlink two error");
	printf("file two unlink");

	exit(0);
}

