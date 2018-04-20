/*************************************************************************
    > File Name: t.cpp
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年04月04日 星期三 09时17分07秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	int num = 0;

	if((pid = vfork()) < 0)
		return 0;
	else if(pid == 0)
	{
		num++;
		printf("hello world! %d\n", num);

		return 0;
	}
	sleep(1);
	printf("hello world%d %ld\n", num, (long)getpid());

	return 0;
}
