# 关于SIGHCLD和SIGCLD

标签（空格分隔）： 信号

---

### SIGCLD

简单的说, SIGCLD不是可靠信号. 这里的不可靠是指当大量信号来的时候, 重复信号它只会处理一次, 而不是多次. 很多信号在瞬间("同时")产生,内核也不一定能够一一处理, 不保证产生的次数.

- - -
### SIGHCLD

* 
> 如果父进程在fork之后调用wait，就会阻塞，直到有一个子进程退出。如果父进程在fork之前先signal(SIGCLD,Fun)，即注册了SIGCLD的信号处理函数。然后做自己的事情。
> 当子进程退出时，会给父进程发送一个SIGCLD信号。然后Fun函数就会执行。可以在Fun函数中调用wait获得子进程退出时的状态，并且此时wait不会阻塞。 
> 当Fun函数执行完后，父进程又继续做自己的事情。

* 
> SIGCHLD简单的说，子进程退出时父进程会收到一个SIGCHLD信号，默认的处理是忽略这个信号，而常规的做法是在这个信号处理函数中调用wait函数获取子进程的退出状态。 

```c++

/*************************************************************************
    > File Name: SIGCHLD_SIGCLD.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月03日 星期六 17时56分10秒
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

    // if(signal(SIGCLD,sig_cld) ==  SIG_ERR)
    if(signal(SIGCHLD,sig_cld) ==  SIG_ERR)
    {
        printf("signal error\n");
        exit(-1);
    }
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
        
        // sig_cld(0);
        int i,j;
        for(i=0;i<100;i++)
        {
            for(j=0;j<10000000;j++)
                ;
            printf("%d\n",i);
        }
        
    }

}

static void sig_cld(int)
{
    pid_t pid;
    int status;

    printf("SIGCLD received\n");
    
    // wait() 返回退出状态
    if((pid = wait(&status)) <0)
    {
        printf("wait error\n");
    }
    
    printf("pid = %d\n",pid);
}
```
运行结果
```
[root@localhost Signal]# ./a.out 
13936 fork a new child 13937
0
1
2
...
51
52
SIGCLD received
pid = 13937
53
54
...
```
从运行就可以看出来, 信号中的函数调用wait()父进程依然在运行, 当子进程结束后, 父进程接着执行. 
当去掉signal那段函数, 在else中调用sig_cld()函数, 结果将发生改变, 当子进程结束, 父进程才继续运行.
