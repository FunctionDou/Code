#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    int fd[2];
    if(pipe(fd) < 0)
        exit(EXIT_FAILURE);
    pid_t pid;
    char buf[1024];

    if((pid = fork()) < 0)
        exit(EXIT_FAILURE);
    else if(pid == 0)
    {
        close(fd[0]);
        //dup2(fd[1], 1);
        int n = read(1, buf, sizeof(buf));
        write(fd[1], buf, n);
        exit(EXIT_SUCCESS);
    }

    close(fd[1]);
    //dup2(fd[0], 0);
    int n = read(fd[0], buf, sizeof(buf));
    write(0, buf, n);



    exit(EXIT_SUCCESS);
}