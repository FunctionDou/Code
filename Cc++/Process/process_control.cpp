/*************************************************************************
    > File Name: process_control.cpp
    > Author: Function_Dou
    > Created Time: 2018年01月18日 星期四 13时35分10秒
 ************************************************************************/

/************************************************************************

    fork() : 通过系统调用创建一个与原来进程几乎完全相同的进程，也就是两个进程可以做完全相同的事，但如果初始参数或者传入的变量不同，两个进程也可以做不同的事。
    一个进程调用fork（）函数后，系统先给新的进程分配资源，例如存储数据和代码的空间。然后把原来的进程的所有值都复制到新的新进程中，只有少数值与原来的进程的值不同。相当于克隆了一个自己。


    pid_t fork(void);

    > fork调用的一个奇妙之处就是它仅仅被调用一次，却能够返回两次，它可能有三种不同的返回值：
     1）在父进程中，fork返回新创建子进程的进程ID；
     2）在子进程中，fork返回0；
     3）如果出现错误，fork返回一个负值；

    进程执行没有固定的先后顺序，哪个进程先执行要看系统的进程调度策略。



    execlp() : 从PATH 环境变量中查找文件并执行
    execlp() 会从PATH 环境变量所指的目录中查找符合参数file的文件名, 找到后便执行该文件, 然后将第二个以后的参数当做该文件的argv[0]、argv[1]……, 最后一个参数必须用空指针(NULL)作结束。
    如果执行成功则函数不会返回,执行失败则直接返回-1,失败原因存于errno 中



    execlp() : 要求参数必须以NULL结尾, 而不是以'\n'结尾. 一般采用 execlp($1, $2, (char *)0)进行将末尾设置为 NULL;
    fork() : 创建一个新进程, 一般在函数中创建是为子进程;
    调用execlp() 以执行从标准输入读入指令, fork()跟execlp()就是操作系产生新进程;
    waitpid() : 指令目前进程要等待的进程. waitpid()返回等待的子进程的终止状态(ststus);
    waitpid()会暂时停止目前进程的执行, 直到有信号来到或子进程结束. 如果在调用wait()时子进程已经结束, 则wait()会立即返回子进程结束状态值. 子进程的结束状态值会由参数status 返回, 而子进程的进程识别码也会一快返回. 如果不在意结束状态值, 则参数status 可以设成NULL. 参数pid 为欲等待的子进程识别码

    与进程相同, 线程也用ID标识. 但是线程ID只在所属的进程内其作用, 在其他进程就没有意义了, 即, 线程属于进程;

*************************************************************************/
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include "apue.h"

int main()
{
    char buf[MAXLINE];
    pid_t pid;
    int status;

    printf("%%");

    while(fgets(buf, MAXLINE, stdin) != NULL)
    {
        if(buf[strlen(buf) - 1] ==  '\n')
            buf[strlen(buf) - 1] = 0;

        if((pid = fork()) < 0)
            err_sys("fork error");
        else if(pid == 0)
        {
            execlp(buf, buf, (char *)0);
            err_ret("couldn't execute: %s", buf);
            exit(127);
        }

        if((pid = waitpid(pid, &status, 0)) < 0)
            err_sys("waitpid error");

        printf("%%");
    }


    return 0;
}

/*****************fork() 的一个示例****************************
#include <unistd.h>
#include <stdio.h>
int main ()
{
    pid_t fpid; //fpid表示fork函数返回的值
    int count=0;
    fpid=fork();
    if (fpid < 0)
        printf("error in fork!");
    else if (fpid == 0)
    {
        printf("i am the child process, my process id is %d\n",getpid());
        printf("我是爹的儿子\n");
        count++;
    }
    else
    {
        printf("i am the parent process, my process id is %d\n",getpid());
        printf("我是孩子他爹\n");
        count++;
    }
    printf("统计结果是: %d\n",count);
    return 0;
}
*/
