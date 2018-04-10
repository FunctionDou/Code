# FIFO 与 共享储存

标签（空格分隔）： 进程通信

---

FIFO有时被称为命名管道. 管道只能由相关进程使用,但是,**通过FIFO,不相关进程也能交换数.** FIFO的路径名存在于文件系统中,一般的文件I/O函数都可用于FIFO.

> 
**FIFO的用途：用于客户进程--服务器进程应用程序中**
FIFO的真正优势在于：服务器可以是一个长期运行的进程（例如守护进程），而且与其客户可以无亲缘关系。
> 
作为服务器的守护进程以某个众所周知的路径名创建一个FIFO，并打开该FIFO来读。此后某个时刻启动的客户打开该FIFO来写，并将其请求通过该FIFO发送出去。（客户到服务器）
每个客户在启动时创建自己的FIFO，所用的路径名含有自己的进程ID。每个客户把自己的请求写入服务器的众所周知的FIFO中，该请求含有客户的进程ID以及一个请求文件路径名，服务器根据客户进程ID可以知道客户FIFO的路径名。
> 
**即每个客户一个子进程服务器。每当有一个客户请求到达时，这种服务器就让主进程调用fork派生出一个新的子进程。该新子进程处理相应的客户请求，直到完成为止。**

**管道和FIFO的特征之一是它们的数据是一个字节流。这是UNIX的原生I/O模型。进程往其中写入的是字节流，系统不对它作解释.**

- - -

### 共享储存

一个例子输出共享储存的实际物理地址. 物理地址估计还是后面再看一遍 csapp 总结后会有深一点的理解. 

```c
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
```
