/*************************************************************************
    > File Name: sem__.cpp
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年03月22日 星期四 22时31分11秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>

static sem_t sem, sem1;
static int num;
static int fd[2];
const int N = 100;

void *thread_fun1(void *)
{
    char s[N];
    int n;
    sem_wait(&sem1);
    while(1)
    {
        n = read(STDIN_FILENO, s, sizeof(s));
        write(fd[1], s, n);
        
        sem_post(&sem);
        sem_wait(&sem1);

    }

    pthread_exit((void *)0);
}

void *thread_fun2(void *)
{
    char m[N];
    int n;
    sem_wait(&sem);
    while(1)
    {
        n = read(fd[0], m, sizeof(m));
        write(STDOUT_FILENO, m, n);

        sem_post(&sem1);
        sem_wait(&sem);
    }
    
    pthread_exit((void *)0);
}


int main()
{
    pthread_t thread1, thread2;
    sem_init(&sem, 0, 0);
    sem_init(&sem1, 0, 1);
    num = 0;
    pipe(fd);

    pthread_create(&thread1, NULL, thread_fun1, NULL);
    pthread_create(&thread2, NULL, thread_fun2, NULL);


    pthread_join(thread2, NULL);
    pthread_join(thread1, NULL);

    exit(0);
}