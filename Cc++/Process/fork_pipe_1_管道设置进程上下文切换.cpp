/*************************************************************************
    > File Name: fork_pipe_1_管道设置进程上下文切换.cpp
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年03月22日 星期四 12时39分23秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

static int fd1[2], fd2[2];
void WAIT_TELL();
void WAIT_fd1();
void WAIT_fd2();
void TELL_fd1();
void TELL_fd2();

int Read();
int Write(const char *s);
void Fprintf(const char *s);

static int num;
int Fork();
void Board();
void Get_off();


int main()
{
    srand(0);
    WAIT_TELL();
    num = 0;
    pid_t driver_id;

    if((driver_id = Fork()) == 0)
    {
        Board();
    }
    else
    {
        Get_off();
    }
    

    exit(0);
}


int Write(const char *s)
{
    int m;
    m = strlen(s);
    write(STDOUT_FILENO, s, m);

    return m;
}


void WAIT_TELL()
{
    if(pipe(fd1) < 0 || pipe(fd2) < 0)
    {
        Fprintf("pipe error");    
    }
}

void WAIT_fd1()
{
    char c;
    if(read(fd1[0], &c, 1) != 1)
        Fprintf("read fd1 error");
    
    if(c != 'c')
        Fprintf("read fd1 error");

}

void TELL_fd1()
{
    if(write(fd1[1], "c", 1) != 1)
        Fprintf("write fd1 error");
}

// void WAIT_fd2()
// {
//     char p;
//     if(read(fd2[0], &p, 1) != 1)
//         Fprintf("write fd2 error");
//     if(p != 'c')
//         Fprintf("write fd2 error");
// }

// void TELL_fd2()
// {
//     if(write(fd2[1], "p", 1) != 1)
//         Fprintf("read fd2 error");
// }




void Fprintf(const char *s)
{
    fprintf(stderr, "%s\n", s);

    exit(1);
}


int Fork()
{
    int n;
    if((n= fork()) < 0) 
    {
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    return n;
}

void Board()
{
    int t = 10;
    while(t--)
    {
        Write("Boarding\n");
        num += (rand() % 5 + 1);
        if(num != 0)
        {
            TELL_fd1();
            //sleep(1);
        }
    }
}
void Get_off()
{
    int n;
    int t = 10;
    while(t--)
    {
        if(num <= 0)
        {
            WAIT_fd1();
            //sleep(1);
        }
        n = rand() % 5;
        num = num - n ? num - n : 0;

        Write("Get_off\n");
    }
}