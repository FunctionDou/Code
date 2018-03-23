/*************************************************************************
    > File Name: t.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月07日 星期三 18时51分14秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include "apue.h"
#include <sys/time.h>

const int Max = 30000;
const int max = Max - 1;
long arr[Max];
long arr1[Max];

void Sort(long *arr, int num)
{
	int i, j, t;
	for (i = 0; i < max; i++)
		for(j = i + 1; j < Max; j++)
			if(num == 0)
			{
				if(arr[i] < arr[j])
				{
					t = arr[i];
					arr[i] = arr[j];
					arr[j] = t;
				}
			}
			else 
				if(arr[i] > arr[j])
				{
					t = arr[i];
					arr[i] = arr[j];
					arr[j] = t;
				}

}

void *SortThread(void *)
{
	Sort(arr, 0);

	pthread_exit((void *)1);
}

void *SortThread2(void *)
{
	Sort(arr1, 1);

	pthread_exit((void *)1);
}

int main(void)
{
	int i;
	struct timeval start, end;
	
	// 线程开始的计时
	gettimeofday(&start, NULL);
    
    // 生成大量随机数, 用于排序
	srandom(1);
	for(i = 0; i < Max; i++)
	{
	    arr[i] = random();
	    arr1[i] = i;
	}
	
	// 一个进行随机排序. 一个最坏情况的排序
	pthread_t threadid1, threadid2;
	pthread_create(&threadid1, NULL, SortThread, NULL);
	pthread_create(&threadid2, NULL, SortThread, NULL);
	
	// 线程的结束时间
	gettimeofday(&end, NULL);
	// 时间的差值计算
	printf("pthread no time is : %.4f\n", (double)(end.tv_sec * 1000000 + end.tv_usec - (start.tv_sec * 1000000 - start.tv_usec)) / 1000000);


// ------------------------------------
    // 没有线程的排序计时
	gettimeofday(&start, NULL);
	
	for(i = 0; i < Max; i++)
        arr[i] = random();
    
    // 一个进行随机排序. 一个最坏情况的排序
	Sort(arr, 0);
	Sort(arr1, 1);
	
	// 线程的结束时间
	gettimeofday(&end, NULL);
	// 时间的差值计算
	printf("pthread no time is : %.4f\n", (double)(end.tv_sec * 1000000 + end.tv_usec - (start.tv_sec * 1000000 - start.tv_usec)) / 1000000);

	exit(0);
}


/*****************  练习1 测试自增 ***************
int i = 1;


void * Fun1(void *)
{
	i++;

	sleep(2);
	
	return ((void *)1);
}

void * Fun2(void *)
{
	i++;

	sleep(2);
	
	return ((void *)1);
}

int main()
{
	int err;
	pthread_t thread, thread2;
	void *tret;

	err = pthread_create(&thread, NULL, Fun1, NULL);
	if(err != 0)
		return 0;
	
	pthread_cancel(thread);
	pthread_join(thread, &tret);

	err = pthread_create(&thread2, NULL, Fun2, NULL);
	if(err != 0)
		return 0;
	pthread_join(thread2, &tret);

	
	printf("%d", i);

	exit(0);

}
*/ 
