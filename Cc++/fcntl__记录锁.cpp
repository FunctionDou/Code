/*
struct flock
{
    short l_type;
    short l_whence;
    off_t l_start;
    off_t l_len;
    pid_t l_pid;
};
l_type：是要获取锁呢，还是释放锁？它的值为 F_RDLCK，F_WRLCK，F_UNLCK.
l_whence : SEEK_SET, SEEK_CUR, SEEK_END 
l_start：相对 l_whence 的偏移 
l_len：要加锁或解释的字节数
l_pid：获取有记录锁的 pid 
加写锁，如果是加读锁，这里就写 F_RDLCK flk.l_type = F_WRLCK;

// 设置参考点，参考 lseek；这里表示参考点为第 0 字节处。
flk.l_whence = SEEK_SET;

// 相对于参考点什么位置；
flk.l_start = start;

// 要加锁的字节数。
flk.l_len = len;

// 获得锁。如果该区域有任何一个字节被其他进程加锁，则阻塞。如果设置 cmd 参数为 F_SETLK，获得所失败会返回 -1，同时 errno 置为 EAGAIN，成功返回 0.
fcntl(fd, F_SETLKW, &flk);
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <fcntl.h>

void Test(int, int, int);

// 记录锁测试
void Test(int fd, int start, int len)
{
    struct flock tmp_lock;
    tmp_lock.l_start = start;
    tmp_lock.l_len = len;
    tmp_lock.l_type = F_WRLCK;
    tmp_lock.l_whence = SEEK_SET;

    // 设置为 F_GETLK 获取记录锁的信息
    // 将信息存放在 l_type, l_pid 中
    if (fcntl(fd, F_GETLK, &tmp_lock) < 0)
    {
        printf("fcntl F_GETLK error\n");
        exit(1);
    }
    // F_UNLCK 没有加记录锁
    if (tmp_lock.l_type == F_UNLCK)
        printf("F_UNLCK\n");
    // 加了记录锁
    else
        printf("locking pid = %d\n", tmp_lock.l_pid);
}

int main(int argc, char *argv[])
{
    printf("pid = %d\n", getpid());

    int fd;
    fd = open(argv[1], O_WRONLY);

    int start, len;
    start = atoi(argv[2]);
    len = atoi(argv[3]);
    struct flock f_lock;
    f_lock.l_start = start;
    f_lock.l_len = len;
    f_lock.l_type = F_WRLCK;
    f_lock.l_whence = SEEK_SET;

    Test(fd, start, len);

    if (fcntl(fd, F_SETLKW, &f_lock) == -1)
    {
        printf("fcntl error\n");
        exit(1);
    }
    else
        printf("running\n");

    sleep(5);
    exit(EXIT_SUCCESS);
}
