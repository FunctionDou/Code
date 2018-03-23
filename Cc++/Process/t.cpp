/*************************************************************************
    > File Name: t.cpp
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年03月22日 星期四 17时20分56秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

static pthread_mutex_t mutex;
static int num;

void *Boarding(void *board);
void *Get_off(void *off);

void Board_printf();
void Get_off_printf();

void Pthread_create(pthread_t *t, void *(*fun)(void *), void *arg);
int Write(const char *s);


int main()
{
    srand(0);
    num = 0;
    pthread_t thread_father, thread_child;

    pthread_mutex_init(&mutex, NULL);    
    Pthread_create(&thread_father, Boarding, NULL);
    Pthread_create(&thread_child, Get_off, NULL);

    pthread_join(thread_father, NULL);
    //sleep(100);
    pthread_mutex_destroy(&mutex);
    exit(0);
}


int Write(const char *s)
{
    int n;
    n = strlen(s);
    if(write(STDOUT_FILENO, s, n) != n)
    {
        fprintf(stderr, "write error\n");
        exit(1);
    }

    return n;
}

// void *Boarding(void *borad)
// {
//     int t = 10;
//     while(t--)
//     {
//         if(num >= 0)
//         {
//             pthread_mutex_lock(&mutex);
//             num = rand() % 5 + 1;
//             Write("Boarding\n");
//             pthread_mutex_unlock(&mutex);
//             sleep(1);
//         }
//     }
//     return ((void *)0);
// }

// void *Get_off(void *off)
// {
//     int t = 10;
//     int n;
//     while(t--)
//     {
//         if(num > 0)
//         {
//             pthread_mutex_lock(&mutex);
//             n = rand() % 5;
//             num = num - n ? num - n : 0;
//             Write("get off\n");
//             pthread_mutex_unlock(&mutex);
//             sleep(1);
//         }
//     }
//     return ((void *)0);
// }

void Pthread_create(pthread_t *t, void *(*fun)(void *), void *arg)
{
    if((errno = pthread_create(t, NULL, fun, arg)) != 0)
    {
        fprintf(stderr, "pthread_create error : %s\n", strerror(errno));
        exit(1);
    }
}
void *Boarding(void *borad)
{
    int t = 10;
    while(t--)
    {
        pthread_mutex_lock(&mutex);
        Board_printf();
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    
    pthread_exit((void *)0);
}

void Board_printf()
{
    if(num >= 0)
    {
        num = rand() % 5 + 1;
        Write("Boarding\n");
    }
}

void *Get_off(void *off)
{
    int t = 10;
    while(t--)
    {
        pthread_mutex_lock(&mutex);
        Get_off_printf();
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
  
    pthread_exit((void *)0);
}

void Get_off_printf()
{
    int n;
    if(num > 0)
    {
        n = rand() % 5;
        num = num - n ? num - n : 0;
        Write("get off\n");
    }
}