#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>

#define addr 0x8888

int get_id(int num)
{
    int id;
    if (num > 0)
    {
        // 第一次申请了64哥字节的空间
        id = shmget(addr, 64, IPC_CREAT | IPC_EXCL | 0664);
        // 把共享内存区映射到进程的地址空间
        char *s = (char *)shmat(id, NULL, 0);
        s = NULL;
        // 链接共享单元
        shmdt(s);
    }
    else
        // 共享单元已经创建的时侯, 进行连接就行, 不用创建了
        id = shmget(addr, 0, 0);

    return id;
}

int main(int argc, char *argv[])
{
    // argv[1] > 0 第一次, 建立共享空间, 并连接
    // argv[0] = 0 直接进行连接
    int id = get_id(atoi(argv[1]));
    char *s = (char *)shmat(id, 0, 0);
    char buf[1024];

    if (argv[2][0] == 'w')
    {
        int i;
        // while (1)
        // {
        read(1, buf, sizeof(buf));
        i = 0;
        while (buf[i] != '\0')
            s[i] = buf[i++];
        s[i] = '\0';
        // }
    }
    else if (argv[2][0] == 'r')
        // while (1)
        // {
        //     if(s[0] == '\0')
        //         continue;
        write(0, s, strlen(s));
    //     s[0] = '\0';
    // }
    else if (argv[2][0] == 'd')
        shmctl(id, IPC_RMID, 0);

    exit(EXIT_SUCCESS);
}