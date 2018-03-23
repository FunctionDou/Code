/*************************************************************************
    > File Name: sem_init_2_进程切换.cpp
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年03月23日 星期五 08时29分03秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>

// 设置两个信号量
sem_t sem_1, sem_2;

// 重新包装线程创建
void Pthread_create(pthread_t *id, void *(*fun)(void *), void *arg)
{
    if((errno = pthread_create(id, NULL, fun,arg)) != 0)
    {
        fprintf(stderr, "pthread_create error : %s\n", strerror(errno));
        exit(1);
    }
}

// 线程 1
void *thread_1(void *)
{
    // 对进程 1 先执行. 因为 sem_1 一开始就是空闲的.
    sem_wait(&sem_1);
    while(1)
    {
        printf("thread 1\n");
        fflush(stdout);
        // 将 sem_2 信号量的 sv++, 线程不再空闲
        sem_post(&sem_2);
        // 等待 线程2 执行完, 阻塞等待
        sem_wait(&sem_1);
        sleep(1);
    }

    pthread_exit((void *)0);
}

void *thread_2(void *)
{
    // 一开始就阻塞, 直到线程1执行 sem_post 才开始执行
    sem_wait(&sem_2);
    while(1)
    {
        printf("2 pthread\n");
        fflush(stdout);
        // 将 sem_1 信号量 ++. 让线程1开始执行
        sem_post(&sem_1);
        // 等待线程1执行完
        sem_wait(&sem_2);
        sleep(1);
    }

    pthread_exit((void *)0);
}

int main()
{
    pthread_t id_1, id_2;

    // 将sem_1信号量初始化为 空闲. 很重要
    // 将sem_2信号量初始化为 忙
    sem_init(&sem_1, 0, 1);
    sem_init(&sem_2, 0, 0);

    Pthread_create(&id_1, thread_1, NULL);
    Pthread_create(&id_2, thread_2, NULL);

    pthread_join(id_1, NULL);
    pthread_join(id_2, NULL);

    exit(0);
}

// static sem_t sem_1;
// static sem_t sem_2;
// int main()
// {
//     pid_t id;
//     // 初始化信号量
//     sem_init(&sem_1, 1, 0);
//     sem_init(&sem_2, 1, 1);

//     if ((id = fork()) < 0)
//         exit(1);

//     // 子进程
//     if (id == 0)
//     {
//         int t = 10;
//         sem_wait(&sem_2);
//         while (t--)
//         {
//             // 信号量值减一
//             printf("0000000000\n");
//             // 信号量值加一
//             sem_post(&sem_1);
//             printf("%d\n", sem_1);
//             sem_wait(&sem_2);
//         }
//         exit(0);
//     }
//     printf("d\n");
//     int m = 10;
//     sem_wait(&sem_1);
//     printf("q\n");
//     while (m--)
//     {
//         // 信号量值减一
//         printf("111111\n");
//         // 信号量值加一
//         sem_post(&sem_2);
//         sem_wait(&sem_1);
//     }
//     exit(0);
// }