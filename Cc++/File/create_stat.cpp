/*************************************************************************
    > File Name: create_stat.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月08日 星期四 19时16分24秒

	头文件：#include <sys/types.h>   #include <sys/stat.h>   #include <fcntl.h>

	定义函数：int creat(const char * pathname, mode_tmode);

	返回值：
	creat()会返回新的文件描述词, 若有错误发生则会返回-1, 并把错误代码设给errno.
	EEXIST参数：pathname 所指的文件已存在.
	EACCESS参数：pathname 所指定的文件不符合所要求测试的权限
	EROFS：欲打开写入权限的文件存在于只读文件系统内
	EFAULT参数：pathname 指针超出可存取的内存空间
	EINVAL参数：mode 不正确.
	ENAMETOOLONG参数：pathname 太长.
	ENOTDIR 参数：pathname 为一目录
	ENOMEM ：核心内存不足
	ELOOP 参数：pathname 有过多符号连接问题.
	EMFILE：已达到进程可同时打开的文件数上限
	ENFILE：已达到系统可同时打开的文件数上限

 ************************************************************************/

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define Max 100
int main(void)
{
	int fd;
	struct stat sta;
	mode_t mode;
	char buf[Max];
	const char *filename = "temp";
	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;


	creat(filename, mode);
	if((fd = open(filename, O_RDWR)) == -1)
	{
		fprintf(stderr, "open error");
		exit(1);
	}
	else
		printf("open sccess\n");

	if(stat(filename, &sta) == -1)
	{
		fprintf(stderr, "stat error : %s", strerror(errno));
	}
	else
	{
		printf("uid = %lu, gid = %lu\n", (unsigned long)sta.st_uid, (unsigned long)sta.st_gid);
		printf("pathname %s\n", getcwd(buf, sizeof(buf)));
	}

	if(unlink(filename) == -1)
	{
		fprintf(stderr, "unlink error\n");
		exit(1);
	}
	fprintf(stdout, "%s file delete sccess\n", filename);

	exit(0);
}
