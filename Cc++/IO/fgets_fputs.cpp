/*************************************************************************
    > File Name: fgets_fputs.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月07日 星期三 23时16分28秒

********************************  fgets  ********************************

	头文件 include<stdio.h>
	
	fgets()函数用于从文件流中读取一行或指定个数的字符，其原型为：

	char * fgets(char * string, int size, FILE * stream);

	tring为一个字符数组，用来保存读取到的字符。
	size为要读取的字符的个数。如果该行字符数大于size-1，则读到 size-1 个字符时结束，并在最后补充' \0'；如果该行字符数小于等于 size-1，则读取所有字符，并在最后补充 '\0'。即，每次最多读取 size-1 个字符。
	stream为文件流指针。

	返回值 : 
	读取成功，返回读取到的字符串，即string；失败或读到文件结尾返回NULL。因此我们不能直接通过fgets()的返回值来判断函数是否是出错而终止的，应该借助feof()函数或者ferror()函数来判断。


**********************************  fputs  ******************************

	头文件：#include <stdio.h>

	fputs()函数用于将指定的字符串写入到文件流中，其原型为：

	int fputs(char * string, FILE * stream);

	【参数】string为将要写入的字符串，stream为文件流指针。

	【返回值】成功返回非负数，失败返回EOF。

	fputs()从string的开头往文件写入字符串，直到遇见结束符 '\0'，'\0' 不会被写入到文件中。

	注意：fputs()可以指定输出的文件流，不会输出多余的字符；puts()只能向 stdout 输出字符串，而且会在最后自动增加换行符。

 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define Max 1000

int main(void)
{
	char s[Max];

	if(fgets(s, sizeof(Max), stdin) != NULL)
		if(fputs(s, stdout) == EOF)
		{
			fprintf(stderr, "fputs error");
			exit(1);
		}


	exit(0);
}

