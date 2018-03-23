/*************************************************************************
    > File Name: umask_chomd_修改umask中文件的权限.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月26日 星期五 21时58分01秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <fcntl.h>

int main(void)
{
	struct stat statbuf;

	if(stat("one", &statbuf) < 0)
		err_sys("stat error for one");
	if(chmod("one", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
		err_sys("chmod error for one");

	if(chmod("two", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)
		err_sys("chmod error for two");

	exit(0);
}
