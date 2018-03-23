/*************************************************************************
    > File Name: t.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月20日 星期二 20时36分51秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#define Max 1000

char ch[Max];

int main(void)
{
	fgets(ch, Max, stdin);
	if(ch[strlen(ch) - 1] == '\n')
		ch[strlen(ch) - 1] = '\0';

	long long num1, num2;
	sscanf(ch, "%lld %lld", &num1, &num2);
	sprintf(ch, "%lld", num1 + num2);
	printf("%s", ch);

	int x = 0;
	long y = 0;
	printf("x = %" PRId32 ", y = %" PRIu64 "\n", x, y);
	// printf("%s", PRIu64);

	printf("%d\n", x);
	exit(0);
}
