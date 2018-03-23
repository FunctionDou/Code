/*************************************************************************
    > File Name: mmap_存储映射.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月26日 星期一 16时47分43秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <fcntl.h>
#include <sys/mman.h>

#define COPYINCR (1024 * 1024 * 1024)

int main(int argc, char *argv[])
{
	int fdin, fdout;
	void *src, *dst;
	size_t copysz;
	struct stat sbuf;
	off_t fsc = 0;

	if(argc != 3)
		err_quit("usage: %s <fromfile><tofile>", argv[0]);

	// read 
	if((fdin = open(argv[1], O_RDONLY)) < 0)
		err_sys("fdin open error %s", argv[1]);

	if((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
		err_sys("fout open error %s", argv[2]);

	if(fstat(fdin, &sbuf) < 0)
		err_sys("error fstat");

	// 设置输出文件长度
	if(ftruncate(fdout, sbuf.st_size) < 0)
		err_sys("ftruncate error");

	while(fsc < sbuf.st_size)
	{
		// 设置最大字节为 1G 
		if((sbuf.st_size - fsc) > COPYINCR)
			copysz = COPYINCR;
		else
			copysz = sbuf.st_size - fsc;
		
		// 文件映射到内存
		if((src = mmap(0, copysz, PROT_READ, MAP_SHARED, fdin, fsc)) == MAP_FAILED)
			err_sys("error mmap src");
		if((dst = mmap(0, copysz, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, fsc)) == MAP_FAILED)
			err_sys("error mmap dst");

		// 将输入缓冲区复制到输出缓冲区中
		memcpy(dst, src, copysz);
		// 解除映射区
		munmap(src, copysz);
		munmap(dst, copysz);
		fsc += copysz;
	}

	exit(0);
}
