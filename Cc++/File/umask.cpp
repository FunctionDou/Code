/*************************************************************************
    > File Name: umask.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月26日 星期五 21时49分56秒

	umask函数为进程设置文件模式创建屏蔽字，并返回以前的值。函数原型为：

	#include <sys/stat.h>

	mode_t umask(mode_t cmask);

	umask是由下表列出的9个常量中的若干个按位“或”构成的

	S_IRUSR       用户读

	S_IWUSR      用户写

	S_IXUSR       用户执行

	S_IRGRP       组读

	S_IWGRP      组写

	S_IXGRP       组执行

	S_IROTH       其他读

	S_IWOTH      其他写

	S_IXOTH       其他执行

 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <fcntl.h>

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

int main(void)
{
	umask(0);
	if(creat("one", RWRWRW) < 0)
		err_sys("creat one error");
	
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if(creat("two", RWRWRW) < 0)
		err_sys("creat two error");

	exit(0);
}
