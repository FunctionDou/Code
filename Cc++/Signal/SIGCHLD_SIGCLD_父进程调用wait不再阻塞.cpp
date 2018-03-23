/*************************************************************************
    > File Name: SIGCHLD_SIGCLD.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月03日 星期六 17时56分10秒

        一般的,父进程在生成子进程之后会有两种情况,一种是父进程继续去做别的事情,另一种是父进程啥都不做,一直在wait子进程退出.SIGCHLD信号就是为这第一种情况准备的,
    它让父进程去做别的事情,而只要父进程注册了处理该信号的函数,在子进程退出时就会调用该函数,在该函数中又可以调用wait得到终止的子进程的状态。
    处理信号的函数执行完后，再继续做父进程的事情.

        如果父进程在fork之后调用wait，就会阻塞，直到有一个子进程退出。
    如果父进程在fork之前先signal(SIGCLD, sig_cld)，即注册了SIGCLD的信号处理函数。然后做自己的事情。
    当子进程退出时，会给父进程发送一个SIGCLD信号。然后sig_cld函数就会执行。
    可以在sig_cld函数中调用wait获得子进程退出时的状态，并且此时wait不会阻塞。 当sig_cld函数执行完后，父进程又继续做自己的事情。

    SIGCHLD信号的含义：
        简单的说，子进程退出时父进程会收到一个SIGCHLD信号，默认的处理是忽略这个信号，而常规的做法是在这个信号处理函数中调用wait函数获取子进程的退出状态。 

******************************* wait  **********************************

    #include <sys/types.h>

    #include <wait.h>

    int wait(int *status)

    函数功能是：父进程一旦调用了wait就立即阻塞自己，由wait自动分析是否当前进程的某个子进程已经退出，
    
    如果让它找到了这样一个已经变成僵尸的子进程，wait就会收集这个子进程的信息，并把它彻底销毁后返回；
    
    如果没有找到这样一个子进程，wait就会一直阻塞在这里，直到有一个出现为止。

 ************************************************************************/
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include<sys/wait.h>

static void sig_cld(int);

int main()
{
    pid_t pid;
    // int status;

    // if(signal(SIGCLD,sig_cld) ==  SIG_ERR)
    /*
    if(signal(SIGCHLD,sig_cld) ==  SIG_ERR)
    {
        printf("signal error\n");
        exit(-1);
    }
    */
    if((pid = fork()) < 0)
    {
        printf("fork error\n");
        exit(-1);
    }
    else if(pid == 0)
    {
        //child
        printf("%d fork a new child %d\n",getppid(),getpid());
        sleep(1);
        exit(0);
    }
    else
    {
        //parent
        int i,j;
        for(i=0;i<100;i++)
        {
            for(j=0;j<10000000;j++)
                ;
            printf("%d\n",i);
        }
        sig_cld(pid);
        
    }

}

static void sig_cld(int)
{
    pid_t pid;
    int status;

    printf("SIGCLD received\n");
    
    if((pid = wait(&status)) <0)
    {
        printf("wait error\n");
    }
    
    printf("pid = %d\n",pid);
}
