/*************************************************************************
    > File Name: tmpnam_tmpfile.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月03日 星期六 17时15分32秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *fp;
	if((fp = tmpfile()) == NULL)
	{
		printf("tmpfile error");
		exit(0);
	}

	char *name;
	tmpnam(name);
	printf("%s\n", name);

	exit(0);
}
