/*************************************************************************
    > File Name: vfork__缓冲.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月31日 星期三 20时03分55秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int val = 10;

int main()
{
	int num;
	num = 0;
	pid_t pid;

	printf("vfork:\n");
	if((pid = vfork()) < 0)
		err_sys("vfork error");
	else if(pid == 0)
	{
		val++;
		num++;
		_exit(0);
	}
	
	printf("pid = %d\nval = %d\nnum = %d\n", pid, val, num);
			

	exit(0);
}

/***********************  修改1  *******************************

  // 删除_exit(0);加上
  exit(0);

************************  修改2  *******************************

  // 删除_exit(0)

****************************************************************/


/*    样例
 *
[root@192 Process]# ./a.out
vfork:
pid = 14178
val = 11
num = 1

[root@192 Process]# ./a.out > tep
[root@192 Process]# cat tep
vfork:
pid = 14197
val = 11
num = 1
*/
