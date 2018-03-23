/*************************************************************************
    > File Name: fork__缓冲.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月31日 星期三 17时36分30秒

	pid_t fork(void)

	1.返回值:
	 子进程返回0
	 父进程返回子进程的ID
	 错误,返回-1

	2.没有固定的先后顺序, 可能先执行父进程,也可能先执行子进程. 这个要看系统的调度.
	3.子进程会复制父进程的数据空间, 堆栈段. 但是父子进程都共用一个正文段.
		fork()后面接一个exec(), 将采用写时复用. 即， 子进程不会复制父进程的所有能复制的数据，而是与父进程共用一个副本数据， 当要修改某段数据的时候才复制这个数据段， 提高了效率。

	
	write两次不同的测验都只输出了一次,printf输出的次数不同.
		1. 采用./a.out输出是因为此时缓冲采用的是行缓冲, 所以当输出printf后exit(0)会将其缓冲冲洗, 而重定向文本就是采用的全缓冲, 所以printf没有被冲洗掉.
		2. write()函数是不带缓冲的, 所以只会输出一次
	子父进程的参数不一样, 因为子进程拷贝了一份数据段, 所以数据也就不同是应当的.

	strlen()与sizeof()的区别:
		strlen() : 采用函数进行遍历得到长度, 并且不包含NULL
		sizeof() : 调用的是缓冲区已知字符串的初始化, 长度是固定的, 但是要包含NULL

 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int num = 10;
char ch[] = "write :";

int main(void)
{
	int var;
	var = 0;
	pid_t pid;

	if(write(STDOUT_FILENO, ch, sizeof(ch) -1) != sizeof(ch) - 1)
		err_sys("write error");

	printf("printf\n");

	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
	{
		var++;
		num++;
	}
	else
		sleep(2);
	printf("num = %d\nvar = %d\n", num, var);

	printf("");
	
	exit(0);
}

/*****************  第一次修改  ******************
// 删除exit(0), 加上
_exit(0);

*****************  第二次修改  ******************
// 删除exit(0), 加上
if(pid == 0)
    exit(0);
else
    _exit(0);
**************************************************/

/*    样例 : ./a.out
write :printf
num = 11
var = 1
num = 10
var = 0

[root@192 Process]# ./a.out > temp
[root@192 Process]# cat temp 
write :printf
num = 11
var = 1
printf
num = 10
var = 0
*/
