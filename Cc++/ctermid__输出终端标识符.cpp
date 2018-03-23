/*************************************************************************
    > File Name: ctermid__输出终端标识符.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月04日 星期日 10时31分49秒

*******************************  fprintf *********************************

	头文件：#include <stdio.h>

	定义函数：int fprintf(FILE * stream, const char * format, ...);

	返回值：关于参数format 字符串的格式请参考printf(). 成功则返回实际输出的字符数, 失败则返回-1, 错误原因存于errno 中.

	stream 可以为 stderr, stdout, stdin.


 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Max 100


/*
static char ctermid_name[L_ctermid];
char * ctermid(char *str)
{
	if(str == NULL)
		str = ctermid_name;
	return strcpy(str, "/dev/tty");
}
*/

int main(void)
{
	char tty_name[Max];
	char *ch;
	
	if((ch = ctermid(tty_name)) == NULL)
		fprintf(stderr, "ctermid error");
	printf("%s\n%s", ch, tty_name);
	


	exit(0);
}
