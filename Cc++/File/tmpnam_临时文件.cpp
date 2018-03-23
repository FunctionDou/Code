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


