/*************************************************************************
    > File Name: t.cpp
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年03月17日 星期六 09时22分56秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main()
{
	int *a;
	a = (int *)malloc(sizeof(int));
	a[0] = 1;
	free(a);
	int *b = (int *)malloc(sizeof(int));
	b[0] = 2;
	printf("%d", a[0]);

	exit(0);
}
