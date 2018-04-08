#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <signal.h>


void fun(int sig)
{
    if (sig == SIGUSR1)
        printf("hello\n");
    else if (sig == SIGCHLD)
        wait(NULL);
}

void alrm(int sig)
{
    if(sig == SIGALRM)
        printf("alarm");
}

void child()
{
    int n = 10;
    while (n--)
    {
        kill(getppid(), SIGUSR1);
        //kill(getppid(), SIGALRM);
        sleep(2);
    }

    exit(0);
}

int main(void)
{
    if(signal(SIGUSR1, fun) == SIG_ERR)
        exit(1);
    if (signal(SIGCHLD, fun) == SIG_ERR)
        exit(1);
    if(signal(SIGALRM, alrm) == SIG_ERR)
        exit(1);

    pid_t pid;
    if ((pid = fork()) == 0)
        child();

    int num = 0;
    while (1)
    {
        write(STDOUT_FILENO, ".", 1);
        num = sleep(10);
        printf("%d\n", num);
    }

    exit(0);
}