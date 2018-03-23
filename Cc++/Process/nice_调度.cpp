/*************************************************************************
    > File Name: nice_调度.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月02日 星期五 11时53分18秒

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

***************************  nice  **************************************

	int nice(int inc);

	nice()用来改变进程的进程执行优先顺序. 参数inc 数值越大则优先顺序排在越后面, 即表示进程执行会越慢. 只有超级用户才能使用负的inc 值, 代表优先顺序排在前面, 进程执行会较快.

	如果执行成功则返回0, 否则返回-1, 失败原因存于errno 中.


****************************  strtol  ***********************************

	long int strtol (const char* str, char** endptr, int base);
	
	strtol() 函数用来将字符串转换为长整型数(long)

	str 为要转换的字符串，endstr 为第一个不能转换的字符的指针，base 为字符串 str 所采用的进制。

	当 base 的值为 0 时，默认采用 10 进制转换，
	若endptr 不为NULL，则会将遇到的不符合条件而终止的字符指针由 endptr 传回；若 endptr 为 NULL，则表示该参数无效，或不使用该参数

******************************  setbuf  *********************************

	void setbuf(FILE *stream, char *buf)

	使得文件打开后, 用户可建立自己设置的缓冲区, 而不使用fopen()打开文件设定的默认缓冲区大小

	buf = 0. 文件I/O不带缓冲

 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <errno.h>
#include <sys/time.h>

// 不同系统的操作
#if defined(MACOS)
#include <sys/limits.h>
#elif defined(SOLARIS)
#include <limits.h>
#elif defined(BSD)
#include <sys/param.h>
#endif


unsigned long long count;
struct timeval end;

void checktime(char *str)
{
	struct timeval tv;
	
	// 获取当前的时间放入tv里面.
	gettimeofday(&tv, NULL);
	if(tv.tv_sec >= end.tv_sec && tv.tv_usec >= end.tv_usec)
	{
		printf("%s %lld\n", str, count);
		exit(0);
	}
}

int main(int argc, char *argv[])
{
	pid_t pid;
	char *s;
	int nzero, ret;
	int adj = 0;

	setbuf(stdout, NULL);

#if defined(NZERO)
	nzero = NZERO;
#elif defined(_SC_NZERO)
	nzero = sysconf(_SC_NZERO);
#else
#error NZERO undefined
#endif

	printf("NZERO = %d\n", nzero);
	if(argc == 2)
		// 转换为对应的整形
		adj = strtol(argv[1], NULL, 0);
	// 获得当前的时间
	gettimeofday(&end, NULL);
	end.tv_sec += 10;
		
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
	{
		s = "child";
		printf("current nice value in child is %d, adjusting by %d\n", nice(0) + nzero, adj);
		errno = 0;

		if((ret = nice(adj)) == -1 && errno != 0)
			err_sys("child set scheduling priority");
		printf("now child nice value in parent is %d\n", nice(0) + nzero);
	}

	else
	{
		s = "parent";
		printf("current nice value in parent is %d\n", nice(0) + nzero);
	}
	for(; ; )
	{
		if(++count == 0)
			err_quit("%s counter wrap", s);
		checktime(s);
	}
}


