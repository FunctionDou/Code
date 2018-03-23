/*************************************************************************
    > File Name: readv_writev_1_散布读_聚集写.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月26日 星期一 17时19分31秒

	#include<sys/uio.h>

	ssize_t readv(int filedes, const struct iovec *iov, int iovcnt);

	ssize_t writev(int filedes, const struct iovec *iov, int iovcnt);

	//若成功则返回已读，写的字节数，若出错则返回-1。
	
	这两个函数的第二个参数是指向iovec结构数组的一个指针：
	struct iovec
	{
		void *iov_base; //starting address of buffer
		size_t iov_len; //size of buffer
	}

 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include "apue.h"

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

int main(void)
{
	char buf1[5], buf2[10];
	struct iovec iov[2];
	iov[0].iov_base = buf1;
	iov[0].iov_len = 5;
	iov[1].iov_base = buf2;
	iov[1].iov_len = 10;

	int fd;
	// 设置该程序的掩码
	umask(0);
	// 创建文件
	if(creat("one.txt", RWRWRW) < 0)
		err_sys("error creat");

	// 打开文件
	if((fd = open("one.txt", O_RDWR)) < 0)
		err_sys("error open");

	// 向文件写数据进去
	char ch[] = "123456789abc\0";
	if(write(fd, ch, strlen(ch)) < 0)
			err_sys("write error");
	// 这里的文件一定要先关闭一次, 不然就不会将数据写入到磁盘中, 而是在缓冲中, 不关闭这样后面的操作就会出问题
	close(fd);

	// 重新打开文件
	if((fd = open("one.txt", O_RDWR)) < 0)
		err_sys("error open");

	// 散布读, 返回读到的字节数
	int sizev = readv(fd, iov, 2);
	printf("readv : %d\n", sizev);
	
	close(fd);

	// 输出在散布读文件中, 数据依次保存到字符串的数据.
	// 验证散布读是先填满前一个字符串, 在填充下一个字符串
	printf("buf1 : %s\nbuf2 : %s\n", buf1, buf2);
	
	// writev(STDOUT_FILENO, iov, 2);

	// 删除文件
	unlink("one.txt");

	exit(0);
}
