/*************************************************************************
    > File Name: read_write_IO效率.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月26日 星期五 00时23分56秒

********************   read   ************************

	#include <unistd>
	
	ssize_t read(int filedes, void *buf, size_t nbytes);
	
	num = read(...);
	num 返回：若成功则返回读到的字节数，若已到文件末尾则返回0，若出错则返回-1
	number：表示系统实际所读取的字符数量。

	filedes：文件描述符
	
	buf:读取数据缓存区

	nbytes:要读取的字节数。


********************  write   **************************

	#include <unistd>

	ssize_t write(int filedes, void *buf, size_t nbytes);
	
	返回：若成功则返回写入的字节数，若出错则返回-1
	
	filedes：文件描述符
	
	buf:待写入数据缓存区
	
	nbytes:要写入的字节数
 
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

#define BUFFSIZE 4096

int main(void)
{
	int n;
	char buf[BUFFSIZE];

	while((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
		if(write(STDOUT_FILENO, buf, n) != n)
			err_sys("write error");

	if(n < 0)
		err_sys("read error");
		

	exit(0);
}
