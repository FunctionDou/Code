/*************************************************************************
    > File Name: timeval_运行时间计算.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月02日 星期五 13时43分54秒

***************************  timeval  ***********************************

	struct timeval
	{
		long tv_sec;  //秒
		long tv_usec;  //微秒
	};

	struct timezone
	{
		int tz_minuteswest;  //和Greenwich 时间差了多少分钟
	    int tz_dsttime;  //日光节约时间的状态
	};

***************************  gettimeofday *******************************

	int gettimeofday (struct timeval * tv, struct timezone * tz);

	gettimeofday 会把目前的时间放入tv, 结构中, 当地时区的信息则放入tz结构中.

	返回值：成功则返回0，失败返回－1，错误代码存于errno。

 ************************************************************************/

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	struct timeval end;
	struct timeval start;

	gettimeofday(&start, NULL);
	
	sleep(2);
	gettimeofday(&end, NULL);

	printf("sec : %ld\nusec : %ld\n", end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);
	
	exit(0);	
}
