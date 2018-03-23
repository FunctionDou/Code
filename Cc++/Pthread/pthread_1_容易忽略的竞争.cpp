/*************************************************************************
    > File Name: pthread_1_容易忽略的竞争.cpp
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年03月21日 星期三 12时52分34秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

const int N = 4;

void Error(const char *s)
{
    fprintf(stderr, "%s : %s\n", s, strerror(errno));
    exit(1);
}

void* thread(void *i)
{
    printf("pid = %d, i = %d\n", getpid(), i);

    pthread_exit((void *)1);
}

int main(void)
{
    int i;
    pthread_t id[4];

    printf("(void *)\n");
    for(i = 0 ; i < N; i++)
    {
        if((errno = pthread_create(&id[i], NULL, thread, (void *)i)) != 0)
            Error("pthread_create error");
    }
    for(i = 0; i < N; i++)
        pthread_join(id[i], NULL);


    printf("\n\n&i\n");
    for(i = 0 ; i < N; i++)
    {
        if((errno = pthread_create(&id[i], NULL, thread, &i)) != 0)
            Error("pthread_create error");
    }
    for(i = 0; i < N; i++)
        pthread_join(id[i], NULL);

    sleep(2);

    exit(0);
}