# 映射I/O

标签（空格分隔）： 标准I/O流

---

### 存储映射I/O

> 
内存映射，简而言之就是将用户空间的一段内存区域映射到内核空间，映射成功后，用户对这段内存区域的修改可以直接反映到内核空间，同样，内核空间对这段区域的修改也直接反映用户空间。那么对于内核空间<---->用户空间两者之间需要大量数据传输等操作的话效率是非常高的。

- **存储映射I/O能将磁盘文件映射到存储空间的一个缓冲区中, 所以当从缓冲区读入数据时, 就像是读取文件中的相应字节.**
- **同理, 将数据写入缓冲区时, 相应字节就会自动的写入到文件中对应的位置.**
- **不必再调用read()，write（）等操作.**
- **这样对于内核空间<---->用户空间两者之间需要大量数据传输等操作的话效率是非常高的.**

> 
当然, 写完后，内存中的内容并不会立即更新到文件中，而是有一段时间的延迟，你可以调用msync()来显式同步一下, 这样你所写的内容就能立即保存到文件里了. 
不过通过mmap来写文件这种方式没办法增加文件的长度, 因为要映射的长度在调用mmap()的时候就决定了.
如果想取消内存映射，可以调用munmap()来取消内存映射

```c
#include <sys/mman.h>

void *mmap(void *addr, size_t len, int prot, int flag, int fd, off_t off);  // 映射
// 成功, 返回映射区的起始地址; 错误, 返回MAP_FAILED.
```
mmap()参数 :
> 
1. start：要映射到的内存区域的起始地址，通常都是用NULL（NULL即为0）。NULL表示由内核来指定该内存地址  
2. length：要映射的内存区域的大小  
3. prot：期望的内存保护标志，不能与文件的打开模式冲突。是以下的某个值，可以通过or运算合理地组合在一起  
* PROT_EXEC //页内容可以被执行  
* PROT_READ  //页内容可以被读取  
* PROT_WRITE //页可以被写入  
* PROT_NONE  //页不可访问  
4. flags：指定映射对象的类型，映射选项和映射页是否可以共享。它的值可以是一个或者多个以下位的组合体  
5. fd：文件描述符(由open函数返回)
6. offset：表示被映射对象（即文件）从那里开始对映，通常都是用0. 该值应该为大小为PAGE_SIZE的整数倍  

```c
// 调用 msync 将该页冲洗到被映射的文件中.
// msync 类似于 fsync , 但作用于存储映射区.
int msync(void *addr, size_t len, int flags)
// 成功, 返回 0; 失败, 返回 -1.
```

```c
// 当进程终止时, 会自动解除存储映射区的映射, 或者直接调用 mumap()函数解除存储映射区的映射.
// 关闭存储映射区时使用的文件描述符并不会解除映射区.
int mumap(void *addr, size_t len)
// 成功, 返回 0; 失败, 返回 -1.
```

书上描述的程序代码
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

运行结果
```c
[root@localhost IO]# cat t
sfasfasf
[root@localhost IO]# cat t1
asfas23
asfaseasf325f43t
[root@localhost IO]# ./a.out t t1
[root@localhost IO]# cat t
sfasfasf
[root@localhost IO]# cat t1
sfasfasf
```

