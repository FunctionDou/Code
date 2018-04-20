#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define myerrno (*__Errno())

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t key;

void Print_once()
{
    printf("once\n");
    // 私有键值的初始化
    pthread_key_create(&key, free);
}

int *__Errno()
{
    int *one;
    // 键值只能初始化一次
    pthread_once(&once, Print_once);
    // 获得键值对应的值
    one = (int *)pthread_getspecific(key);
    if (one == NULL)
    {
        // 分配空间
        one = (int *)malloc(sizeof(int));
        // 将键值对保存到自己私有的线程空间中
        pthread_setspecific(key, (void *)one);
    }
    return one;
}

void *Fun_thread(void *)
{
    myerrno = 0;
    printf("hello thread %d\n", myerrno);
}

void *Fun_thread_(void *)
{
    myerrno = 50;
    printf("Fun_thread_ %d\n", myerrno);
}

const int num = 10;

int main(int argc, char *argv[])
{
    pthread_t thread_arr[num];
    pthread_create(&thread_arr[0], NULL, Fun_thread, NULL);
    pthread_create(&thread_arr[1], NULL, Fun_thread_, NULL);
    pthread_join(thread_arr[0], NULL);
    pthread_join(thread_arr[1], NULL);

    pthread_key_delete(key);

    exit(EXIT_SUCCESS);
}