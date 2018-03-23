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
