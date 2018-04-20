// select.c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define PERR(msg)    \
    do               \
    {                \
        perror(msg); \
        exit(1);     \
    } while (0);

int process(char *prompt, int fd)
{
    int n;
    char buf[64];
    char line[64];
    n = read(fd, buf, 64);
    if (n < 0)
    {
        // read 执行出错
        PERR("read");
    }
    else if (n == 0)
    {
        // 如果对端关闭，read 返回 0
        sprintf(line, "%s closed\n", prompt);
        puts(line);
        return 0;
    }
    else if (n > 0)
    {
        buf[n] = 0;
        sprintf(line, "%s say: %s", prompt, buf);
        puts(line);
    }
    return n;
}

int main()
{
    int n, res;
    char buf[64];

    fd_set st;
    FD_ZERO(&st);

    int fd0 = STDIN_FILENO;
    int fd1 = open("a.fifo", O_RDONLY);
    printf("open pipe: fd = %d\n", fd1);
    int fd2 = open("b.fifo", O_RDONLY);
    printf("open pipe: fd = %d\n", fd2);

    FD_SET(fd0, &st);
    FD_SET(fd1, &st);
    FD_SET(fd2, &st);

    // 最后一个 open 的描述符值是最大的
    int maxfd = fd2 + 1;

    while (1)
    {
        // 因为 tmpset 参数会被 select 修改，所以要重新赋值。
        fd_set tmpset = st;
        res = select(maxfd, &tmpset, NULL, NULL, NULL);

        if (res < 0)
        {
            // select 执行出错，对于被信号中断的，需要单独处理，这里暂时不考虑，后面的文章会讲
            PERR("select");
        }
        else if (res == 0)
        {
            // 超时，先不用管
            continue;
        }

        // 判断返回的集合中是否包含对应的描述符，如果包含，说明的事件（可读）到来。
        if (FD_ISSET(fd0, &tmpset))
        {
            n = process("fd0", fd0);
            // 如果返回值为 0，表示对端关闭，后面的也一样。
            if (n == 0)
                FD_CLR(fd0, &st);
        }
        if (FD_ISSET(fd1, &tmpset))
        {
            n = process("fd1", fd1);
            if (n == 0)
                FD_CLR(fd1, &st);
        }
        if (FD_ISSET(fd2, &tmpset))
        {
            n = process("fd2", fd2);
            if (n == 0)
                FD_CLR(fd2, &st);
        }
    }

    exit(EXIT_FAILURE);
}