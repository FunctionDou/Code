/*************************************************************************
    > File Name: strtol_字符转为整形.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月02日 星期五 12时36分17秒

****************************  strtol  ***********************************

	long int strtol (const char* str, char** endptr, int base);

	strtol() 函数用来将字符串转换为长整型数(long)

	str 为要转换的字符串，endstr 为第一个不能转换的字符的指针，base 为字符串 str 所采用的进制。

	当 base 的值为 0 时，默认采用 10 进制转换，
	若endptr 不为NULL，则会将遇到的不符合条件而终止的字符指针由 endptr 传回；若 endptr 为 NULL，则表示该参数无效，或不使用该参数

 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char str[] = "2356 1111 1a 111";

	char *endptr;

	long one = strtol(str, &endptr, 8);
	long two = strtol(endptr, &endptr, 10);
	long three = strtol(endptr, &endptr, 16);
	long four = strtol(endptr, NULL, 0);
	
	printf("one = %ld\ntwo = %ld\nthree = %ld\nfour = %ld\n", one, two, three, four);

	exit(0);
}

/*    样例
 *
 one = 1262
two = 1111
three = 26
four = 111

*/
