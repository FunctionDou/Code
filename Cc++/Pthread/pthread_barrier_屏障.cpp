/*************************************************************************
    > File Name: pthread_barrier_屏障.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月08日 星期四 15时07分25秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

static pthread_barrier_t barrier;
static pthread_mutex_t mutex;

void *thread1(void *)
{
    pthread_mutex_lock(&mutex);
    printf("thread\n");
    pthread_mutex_unlock(&mutex);
    
    // 线程到达后阻塞
    pthread_barrier_wait(&barrier);
    printf("barrier\n");
}


const int num = 100;
int main(int argc, char *argv[])
{
    pthread_t n[num];
    // 设置屏障数
    pthread_barrier_init(&barrier, NULL, num + 1);
    int i = 0;
    for(; i < num; i++)
        pthread_create(&n[i], NULL, thread1, NULL);

    pthread_barrier_wait(&barrier);
    // 所有线程达到barriet后才开始运行
    printf("main barrier\n");
    
    pthread_barrier_destroy(&barrier);
    for(i = 0; i <num; i++)
        pthread_join(n[i], NULL);

    exit(EXIT_SUCCESS);
}