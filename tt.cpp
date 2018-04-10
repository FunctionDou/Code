#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <pthread.h>

const int addr = 0x8888;

struct Produce_Consume
{
    int food;
    pthread_mutex_t mutex;
};

int main(int argc, char *argv[])
{
    int id;
    // 创建一个共享空间
    if(argv[1][0] == 'i')
        id = shmget(addr, sizeof(Produce_Consume), IPC_CREAT | IPC_EXCL | 0664);
    else 
        id = shmget(addr, 0, 0);

    // 设置一个线程属性
    pthread_mutexattr_t mutexattr;
    // 初始化属性
    pthread_mutexattr_init(&mutexattr);
    // 将线程属性设置为设置为在不同进程间进行同步
    pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED);
    
    //  连接到共享内存
    struct Produce_Consume *pro_con = (struct Produce_Consume *)shmat(id, NULL, 0);
    
    // 先初始化生产的数量
    pro_con->food = 2;
    // 初始化结构体中的互斥锁属性
    pthread_mutex_init(&pro_con->mutex, &mutexattr);
    // 删除不用的属性了
    pthread_mutexattr_destroy(&mutexattr);

    // 将内存进行映射
    shmdt((void *)pro_con);

    // 删除共享内存
    if(argv[1][0] == 'd')
        shmctl(id, IPC_RMID, NULL);

    exit(EXIT_SUCCESS);
}