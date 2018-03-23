/*************************************************************************
    > File Name: pthread_2_容易忽略的竞争.cpp
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年03月21日 星期三 13时15分59秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <time.h>

const int N = 5;

void Error(const char *s)
{
    fprintf(stderr, "%s : %s\n", s, strerror(errno));
    exit(1);
}

void *Rand(void *)
{
    printf("rand = %d\n", rand());

    pthread_exit((void *)0);
}

void *Rand1(void *)
{
    srand(0);
    printf("rand = %d\n", rand());

    pthread_exit((void *)0);
}

int main(void)
{
    int i;  srand(0);
    pthread_t id[N];

    printf("Rand\n");
    for(i = 0; i < N; i++)
    {
        if((errno = pthread_create(&id[i], NULL, Rand, NULL)) != 0)
            Error("pthread_create error");
    }
    for(i = 0; i < N; i++)
        pthread_join(id[i], NULL);

    printf("\n\nRand1\n");
    for(i = 0; i < N; i++)
    {
        if((errno = pthread_create(&id[i], NULL, Rand1, NULL)) != 0)
            Error("pthread_create error");
    }
    for(i = 0; i < N; i++)
        pthread_join(id[i], NULL);

    srand(0);
    printf("\n\nnot thread rand\n");
    for(i = 0; i < N; i++)
        printf("rand = %d\n", rand());

    exit(0);
}
