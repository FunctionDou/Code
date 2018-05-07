#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <pthread.h>

struct Produce_Consume
{
    int food;
    pthread_mutex_t mutex;
} * pro_con;

const int addr = 0x8888;

// 生产者
void *Pro(void *)
{
    while (1)
    {
        pthread_mutex_lock(&pro_con->mutex);
        if(pro_con->food < 5)
            pro_con->food++;
        printf("Produce food %d\n", pro_con->food);
        pthread_mutex_unlock(&pro_con->mutex);
        usleep(1000 * 500);
    }

    return ((void *)0);
}

// 消费者
void *Con(void *)
{
    while (1)
    {
        pthread_mutex_lock(&pro_con->mutex);
        if(pro_con->food > 0)
            pro_con->food--;
        printf("Consume food %d\n", pro_con->food);
        pthread_mutex_unlock(&pro_con->mutex);
        usleep(1000 * 500);
    }
    return ((void *)0);
}

int main(int argc, char *argv[])
{
    int id = shmget(addr, 0, 0);
    // 获取共享内存数据
    pro_con = (struct Produce_Consume *)shmat(id, NULL, 0);

    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, Pro, NULL);
    pthread_create(&thread2, NULL, Con, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    exit(EXIT_SUCCESS);
}