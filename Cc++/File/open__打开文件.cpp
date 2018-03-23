/*************************************************************************
    > File Name: open__打开文件.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月08日 星期四 18时58分11秒

	
	头文件：#include <sys/types.h>    #include <sys/stat.h>    #include <fcntl.h>

	定义函数：
    int open(const char * pathname, int flags);
    int open(const char * pathname, int flags, mode_t mode);

	返回值：若所有欲核查的权限都通过了检查则返回0 值, 表示成功, 只要有一个权限被禁止则返回-1.



	O_RDONLY 以只读方式打开文件
	O_WRONLY 以只写方式打开文件
	O_RDWR 以可读写方式打开文件. 
	上述三种旗标是互斥的, 也就是不可同时使用, 但可与下列的旗标利用OR(|)运算符组合.


	S_IRWXU00700 权限, 代表该文件所有者具有可读、可写及可执行的权限.
	S_IRUSR 或S_IREAD, 00400 权限, 代表该文件所有者具有可读取的权限.
	S_IWUSR 或S_IWRITE, 00200 权限, 代表该文件所有者具有可写入的权限.
	S_IXUSR 或S_IEXEC, 00100 权限, 代表该文件所有者具有可执行的权限.
	S_IRWXG 00070 权限, 代表该文件用户组具有可读、可写及可执行的权限.
	S_IRGRP 00040 权限, 代表该文件用户组具有可读的权限.
	S_IWGRP 00020 权限, 代表该文件用户组具有可写入的权限.
	S_IXGRP 00010 权限, 代表该文件用户组具有可执行的权限.
	S_IRWXO 00007 权限, 代表其他用户具有可读、可写及可执行的权限.
	S_IROTH 00004 权限, 代表其他用户具有可读的权限
	S_IWOTH 00002 权限, 代表其他用户具有可写入的权限.
	S_IXOTH 00001 权限, 代表其他用户具有可执行的权限.


 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int f;
	const char *filename = "temp";
	mode_t mode;
	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

	if((f = open(filename, O_RDONLY)) == -1)
	{
		if((f = open(filename, O_RDWR | O_CREAT, mode)) != -1)
			printf("create success\n");
		else
		{
			if(write(STDOUT_FILENO, "create error\n", 13) != 13)
				exit(1);
			exit(1);
		}

	}
	else
		printf("open success\n");

	close(f);


	exit(0);
}
