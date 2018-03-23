/*************************************************************************
    > File Name: TELL_WAIT_竞争条件.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月31日 星期三 22时38分46秒


	int putc(int ch, FILE *stream);
	参数ch表示要输入的位置，参数stream为要输入的流。
	若正确，返回输入的的字符，否则返回EOF。

 ************************************************************************/

#include <stdio.h>
#include "apue.h"

static void charatatime(char *);

int main(void)
{
	pid_t pid;

	// 修改方法2
	// TELL_WAIT();

	// 修改方法1 
	// if((pid = vfork()) < 0)  没有竞争关系了， 因为先进行子进程
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
	{
		// 修改方法2
		// WAIT_PARENT();
		charatatime("one chile\n");
	}
	else
	{
		// 修改方法2
		// TELL_CHILD(pid);
		charatatime("first parent\n");
	}

	exit(0);
}

static void charatatime (char *str)
{
	char *ptr;
	int c;

	setbuf(stdout, NULL);
	for(ptr = str; (c = *ptr++) != NULL; )
		putc(c, stdout);

}


/*          样例
[root@192 Process]# ./a.out 
first oparent
ne chile
[root@192 Process]# ./a.out 
first paronent
e chile
[root@192 Process]# ./a.out 
first parenot
ne chile
*/


