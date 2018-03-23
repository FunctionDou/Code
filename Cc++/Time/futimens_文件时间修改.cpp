/*************************************************************************
    > File Name: futimens_文件时间修改.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月26日 星期五 22时29分19秒

********************   timespec  ********************

	struct timespec  
	{  
	    long int tv_sec;    //秒 s  
		long int tv_nsec;   //纳秒 ns  
	};  
	
	精确到纳秒

	printf("%ld %ld\n", times[0], times[1]);


*********************** 	扩展   *************************
	struct timeval结构体
	
	头文件 : #include <sys/time.h>

	struct timeval
	{
		__time_t tv_sec;         Seconds(秒)
	    __suseconds_t tv_usec;   Microseconds(微秒)
	};

	其中，tv_sec为1970年1月1日0时0分0秒到now的的秒数，tv_usec为微秒数，即秒后面的零头。
	

	#include <sys/time.h>
	#include <stdio.h>
	int
	main(void)
	{
        int i;
        struct timeval tv;

        for(i = 0; i < 4; i++)
        {
                gettimeofday(&tv, NULL);
                printf("%d\t%d\n", tv.tv_usec, tv.tv_sec);
                sleep(2);
        }

        return 0;
	}
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int i, fd;
	struct stat statbuf;
	struct timespec times[2];

	for(i = 1; i < argc; i++)
	{
		if(stat(argv[i], &statbuf) < 0)
		{
			err_ret("%s stat error", argv[i]);
			continue;
		}

		if((fd = open(argv[i], O_RDWR | O_TRUNC)) < 0)
		{
			err_ret("%s : open error", argv[i]);
			continue;
		}

		times[0] = statbuf.st_atim;
		times[1] = statbuf.st_ctim;
	

		if(futimens(fd, times ) < 0)
			err_ret("%s : futimes error");

		close(fd);
	}

	exit(0);
}
