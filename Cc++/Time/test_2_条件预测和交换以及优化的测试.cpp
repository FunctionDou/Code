/*************************************************************************
    > File Name: t.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月08日 星期四 16时26分08秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
#include <iostream>
#include <algorithm>
using namespace std;

#define Max 100000000

struct Test
{
	double i, j;
	float a, c;
	int num1, num2;
};

void Swap1(int *num1, int *num2)
{
	int t1 = *num1, t2 = *num2, t;
	t = t2;
	t2 = t1;
	t1 = t;

	*num1 = t2;
	*num2 = t1;

	/*
	int t1 = *num1;
	int t2 = *num2;
	int t3 = *num1;
	int t4 = *num2;
	int t5 = *num1;
	*/ 
}

void Swap2(int *num1, int *num2)
{
	
	int t = *num1;
	*num1 = *num2;
	*num2 = t;
	

	// *num1 = *num2, *num1 = *num2, *num1 = *num2, *num1 = *num2, *num1 = *num2;
}


void Swap3(int *num1, int *num2)
{
	asm __volatile__(
			"movl %0, %%eax;"
			"movl %1, %%ebx;"
			"movl %%eax, %1;"
			"movl %%ebx, %0;"
			:"=r"(*num1), "=r"(*num2)
			:"r"(*num1), "r"(*num2)
			:"%eax", "%ebx"
			);
}

/*
void Swap3(int &num1, int &num2)
{
	int t = num1;
	num1 = num2;
	num2 = t;
}
*/ 
int main(void)
{
	int i;
	struct Test test;
	struct timeval start, end;
	int num1 = 111, num2 = 2222;
	// test.num1 = 999999; test.num2 = 999998;

	// 预判断测试
	gettimeofday(&start, NULL);
	for(i = 0; i < Max; ++i)
		if(test.num1 > test.num2)
			;
	gettimeofday(&end, NULL);
	printf(" < time is : %.4f\n", (double)(end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / 1000000);


	gettimeofday(&start, NULL);
	for(i = 0; i < Max; ++i)
		if(test.num1 < test.num2)
			;
	gettimeofday(&end, NULL);
	printf(" > time is : %.4f\n", (double)(end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / 1000000);


	gettimeofday(&start, NULL);
	int t = test.num1 - test.num2;
	for(i = 0; i < Max; ++i)
		if(t)
			;
	gettimeofday(&end, NULL);
	printf("time is : %.4f\n", (double)(end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / 1000000);
	

	// swap 交换测试
	gettimeofday(&start, NULL);
	for(i = 0; i < Max; ++i)
		Swap1(&num1, &num2);
	gettimeofday(&end, NULL);
	printf(" Swap1 time is : %.4f\n", (double)(end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / 1000000);


	gettimeofday(&start, NULL);
	for(i = 0; i < Max; ++i)
		Swap2(&num1, &num2);
	gettimeofday(&end, NULL);
	printf(" Swap2 time is : %.4f\n", (double)(end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / 1000000);

	
	gettimeofday(&start, NULL);
	for(i = 0; i < Max; ++i)
		Swap3(&num1, &num2);
	gettimeofday(&end, NULL);
	printf(" Swap3 time is : %.4f\n", (double)(end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / 1000000);
	
	
	exit(0);
}
