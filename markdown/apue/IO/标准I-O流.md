# 标准I/O流

标签:标准I/O流
---

## 流
流的定向决定了读,写的字符是单字符还是多字符.
<wchar.h>定义为宽字符.
只有两个函数可以改变流的定向.
1. freopen() : 清除一个流的定向
2. fwrite() : 设置流的定向

> int fwrite(FILE *fp, int mode);

>* 流是宽字符, 返回正值
>* 流是单字符, 返回负值
>* 流是未定向. 返回0
注意 : 没有错误返回

- - -
#### open函数
>* fopen : 打开路径名为pathname的文件.
>* freopen : 在一个指定的流上打开依法额指定的文件. 此函数一般用于将一个指定文件打开为一个预订的流 : 标准输入, 标准输入, 标准错误.
>* fdopen : 常用于创建管道, 网络通道函数返回的描述符. 这些特殊的类型文件不能用标准fopne打开.

fopen返回一个FILE指针, FILE指针是指向FILE结构体的, 它包含了I/O流所需要的所有信息.
```
struct _iobuf {
    char *_ptr;     // 缓冲区当前指针
    int _cnt;       // 
    char * _base;   // 缓冲区基址
    int _flag;      // 文件读写模式
    int _file;      // 文件描述符
    int _charbuf;   // 缓冲区剩余的个数
    int _bufsize;   // 缓冲区大小
    char *_tmpfname;
};
typedef struct _iobuf FILE;
```
>* 进程通过系统调用open()打开一个文件, 实际上是获得该文件的文件描述符, 进而通过调用系统的sys_open()实现.
>* 在打开文件时, 会创建一个file对象, 并将指向该对象的指针存入进程描述表中, 进而确定打开文件的文件描述符(也就是数组的下表来确定的)(通俗一点就是文件描述符是确定指针的下标)
>* 创建file对象时, 将file对象与f_op指向了所属文件的操作函数集file_operation, 就将虚拟文件系统与实际文件系统衔接了起来.
具体结构可参考 : http://blog.csdn.net/qq_34992845/article/details/71446333

- - - 
#### fclose()

>* int fclose(FILE *fp);
> 调用此函数时, 会关闭所有与该文件相关联的进程, 冲洗缓冲区, 同时也会释放所有的记录锁.


- - -
#### 读写流
getc() : 可被定义为宏. 
fgetc()是一个函数.
ungetc() : 从流中读取数据以后, 可以使用ungetc函数将字符在压送回流中.压送回流中的字符以后可以从流中读取, 但读的字符顺序与压入的相反. 原因是 : 用ungetc压回字符时, 并没有将它写到低层文件中或者时设备上, 只是将他们写回标准I/O库的缓冲区中.(通俗就是, ungetc写回字符是放到缓冲区中, 而不是内核中, 磁盘上.)
值得注意的是 : 不管是文件达到了文件尾部还是发生了错误, 都会返回相同的值, 所以需要ferror, feof函数来区分.
>* int ferror(FILE *fp);
>* int feof(FILE *fp);
> 两者都是 : 条件为真, 返回!0; 否则返回0.

fgets, fputs通常需要复制两次数据 : 
1. 在内核和标准I/O缓冲区间
2. 标准I/O缓冲区和用户的缓冲区之间
另一种高效的复制是只复制文件的指针; grep就是直接复制的指针.
fgets在每次调用结束后自动的回清洗缓冲.



## 缓冲

>* 全缓冲 
>* 行缓冲, 必须从内核获取数据, 否则不是行缓冲. 指向终端的流.
>* 不带缓冲. 标准错误流stderr一般是不带缓冲的.

- - - 
>* fflush : 冲洗一个流.
>* 在I/O方面 : 将缓冲区的内容写到磁盘中, 写入内核.
>* 在驱动方面 : 冲洗掉流, 丢弃掉流中的数据.

## 临时文件

```
/*************************************************************************
    > File Name: tmpnam_临时文件.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月27日 星期六 10时45分36秒


******************************  tmpnam  *********************************
	char *tmpnam(char *str) 生成并返回一个有效的临时文件名，该文件名之前是不存在的。如果 str 为空，则只会返回临时文件名.
	str -- 这是一个指向字符数组的指针，其中，临时文件名将被存储为 C 字符串.

	1. 一个指向 C 字符串的指针，该字符串存储了临时文件名.如果 str 是一个空指针，则该指针指向个内部缓冲区，缓冲区在下一次调用函数时被覆盖.
	2. 如果 str 不是一个空指针，则返回 str。如果函数未能成功创建可用的文件名，则返回一个空指针.

******************************  tmpfile  ********************************

	FILE *tmpfile(void) 以二进制更新模式(wb+)创建临时文件。被创建的临时文件会在流关闭的时候或者在程序终止的时候自动删除.

	FILE *tmpfile(void)

	如果成功，该函数返回一个指向被创建的临时文件的流指针。如果文件未被创建，则返回 NULL。
	
	如果临时文件被立即使用，那么我们可以同时使用tmpfile函数来对其命名并且打开.


*****************************  rewind  **********************************

	rewind()函数用于将文件指针重新指向文件的开头，同时清除和文件流相关的错误和eof标记，相当于调用fseek(stream, 0, SEEK_SET)，其原型如下：

	void rewind(FILE * stream);

	typedef struct
	{
		int _fd;  // 文件号
	    int _cleft;  // 缓冲区中剩下的字节数
	    int _mode;  // 文件操作模式
	    char * _nextc;  // 下一个字节的位置
	    char * _buff;  // 文件缓冲区位置
	}FILE;

	每当进行一次读写后，该指针自动指向下一次读写的位置。当文件刚打开或创建时，该指针指向文件的开始位置。可以用函数ftell()获得当前的位置指针，也可以用rewind()/fseek()函数改变位置指针，使其指向需要读写的位置.

 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int main(void)
{
	char name[L_tmpnam], line[MAXLINE];
	FILE *fp;

	printf("%s\n", tmpnam(NULL));

	tmpnam(name);
	printf("%s\n", name);

	if((fp = tmpfile()) == NULL)
		err_sys("tmpfile error");

	fputs("one line of output\n", fp);
	rewind(fp);
	if(fgets(line, sizeof(line), fp) == NULL)
		err_sys("fgets error");
	fputs(line, stdout);


	exit(0);
}
```

```
/*************************************************************************
    > File Name: mkstemp_临时文件.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月27日 星期六 11时15分31秒


*****************************  mkstemp  *********************************
	
	int mkstemp(char *template);
	
	mkstemp函数在系统中以唯一的文件名创建一个文件并打开，而且只有当前用户才能访问这个临时文件，并进行读、写操作
	
	与tmpnam相似，所不同的是我们可以为临时文件名指定一个模板，这样我们就可以更好的控制其位置与名字.
	
	char *mktemp(char *template);
	int mkstemp(char *template);
	
	mktemp函数由指定的模板生成一个唯一的文件名。template必须以6个x开始。mktemp函数使用唯一可用的文件名字符来替换这些x字符。他返回一个指向所生成的字符串的指针，如果不可以生成一个唯一的文件名，则会返回一个null指针。
	mkstemp函数在创建与打开临时文件方面与tmpfile相类似。文件名由与mktemp相同的方式生成的，但是返回的结果是一个打开的，底层文件描述符
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <errno.h>

void make_temp(char * templat);

int main(void)
{
	char good_template[] = "abcXXXXXXX";
	char *two = "ABCXXXXXX";

	printf("creat one : ");
	make_temp(good_template);

	printf("\ncreat two : ");
	make_temp(two);

	exit(0);
}

void make_temp(char * templat)
{
	int fd;
	struct stat sbuf;

	if((fd = mkstemp(templat)) < 0)
		err_sys(" creat error");

	close(fd);
	
	if(stat(templat, &sbuf) < 0)
		if(errno == ENOENT)
			printf("file doesn't exist\n");
		else
			err_sys("stat filed");
	else
	{
		printf("file exist\n");
		unlink(templat);
	}

}
```
突然考虑到有些人没有啊apue.h这个文件, 可以把apue.h删除, 然后在文件里面加上这样一段代码. 这个是最简单的解决方法了, 不用大改代码.
```
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdarg.h>

void err_sys(const char *fmt, ...)
{
    printf("%s\n",fmt);

    exit(1);
}
```
