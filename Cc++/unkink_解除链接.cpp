/*************************************************************************
    > File Name: unkink_解除链接.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月26日 星期五 22时15分18秒

	相关函数：link, rename, remove

	头文件：#include <unistd.h>

	定义函数：int unlink(const char * pathname);

	函数说明：unlink()会删除参数pathname 指定的文件. 如果该文件名为最后连接点, 但有其他进程打开了此文件, 则在所有关于此文件的文件描述词皆关闭后才会删除. 如果参数pathname 为一符号连接, 则此连接会被删除。

	返回值：成功则返回0, 失败返回-1, 错误原因存于errno

 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <fcntl.h>

int main(int argc, char *argv[])
{
	if(argc < 2)
		err_sys("input <pathname>");

	int i;
	for(i = 1; i < argc; i++)
		if(unlink(argv[i]) < 0)
			err_sys("unlink %s error\n", argv[i]);
		else
			printf("%s unlink OK\n", argv[i]);

	exit(0);
}
