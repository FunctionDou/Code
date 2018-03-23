/*************************************************************************
    > File Name: t.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月03日 星期六 17时58分22秒

*****************************  sigprocmask ******************************

     #include <signal.h>

     int sigprocmask(int how, const sigset_t *restrict set, sigset_t *restrict oset)

     // 成功, 返回 0; 失败, 返回 -1.
     
     
     参数:

     how可设置的参数为：SIG_BLOCK， SIG_UNBLOCK，SIG_SETMASK
     
     SIG_BLOCK ：按照参数  set 提供的屏蔽字，**屏蔽信号**。并将原信号屏蔽保存到oldset中。

     SIG_UNBLOCK ：按照参数 set 提供的屏蔽字进行信号的**解除屏蔽**。针对set中的信号进行解屏。

     SIG_SETMASK ： 按照参数 set 提供的信号设置重新设置系统信号。即， 该进程新的屏蔽字是 set 的值。

     1. sigprocmask(SIG_BLOCK, set, NULL)
	  屏蔽set中保存的信号
	  可更新程序的信号集
     2. sigprocmask(SIG_UNBLOCK, set, NULL)
	  当前的程序中set中包含的信号全部取消屏蔽
     3. sigprocmask(SIG_SETMASK, set, NULL)
	  程序的屏蔽字为set信号集中包含的信号
	  可更新程序的信号集

 ************************************************************************/

#include <stdio.h>
#include <signal.h>
#include "apue.h"
void checkset(int i);    

int main()    
{
     sigset_t blockset;    
     sigemptyset(&blockset);    

     // 添加两个信号进入信号集中
     sigaddset(&blockset,SIGINT);    
     sigaddset(&blockset,SIGTSTP);    

     // 测试信号集中由SIGINT信号了
     if(sigismember(&blockset,SIGINT))    
	  printf("SIGINT\n\n");    
     // 没有信号输出是因为当前的信号集还是NULL(空的)
     checkset(0);    


     // 将blockset信号集变为当前程序的信号集
     sigprocmask(SIG_SETMASK, &blockset,NULL);    
     if(sigismember(&blockset,SIGINT))    
	  printf("SIGINT\n\n");    
     // 现在的信号集不再为NULL了.
     checkset(1);    


     sigaddset(&blockset,SIGTERM);   
     // sigprocmask(SIG_SETMASK, &blockset,NULL); 
     // 此时的SIG_BLOCK意义跟SIG_SETMASK意义一样, 都是更新当前blockset信号集:
     sigprocmask(SIG_BLOCK,&blockset,NULL);    
     checkset(2);    


     // sigdelset(&blockset,SIGTERM);     
     //sigprocmask(SIG_SETMASK,&blockset,NULL);    
     //sigprocmask(SIG_BLOCK,&blockset,NULL);
     // SIG_UNBLOCK 将blockset 信号集全部取消屏蔽. 也就是程序取消了对信号集的屏蔽了, 但是blockset中的信号还是存在的.
     sigprocmask(SIG_UNBLOCK,&blockset,NULL);
     if(sigismember(&blockset,SIGTERM))    
	  printf("SIGTERM\n");    
     if(sigismember(&blockset,SIGINT))    
	  printf("SIGINT\n");    
     if(sigismember(&blockset,SIGTSTP))    
	  printf("SIGSTP\n\n");    
     
     // 检测到程序中的信号屏蔽已经没有了.
     checkset(3);

     if(sigismember(&blockset,SIGTERM))    
	  printf("SIGTERM\n");    
     if(sigismember(&blockset,SIGINT))    
	  printf("SIGINT\n");    
     if(sigismember(&blockset,SIGTSTP))    
	  printf("SIGTSTP\n");
     checkset(4);
}

void checkset(int i)
{

     sigset_t set;
     printf("check set start:%d\n", i);

     if(sigprocmask(SIG_SETMASK, NULL, &set) < -1)
     {
       printf("check set sig procmask error!!\n");
       exit(0);
     }

     if(sigismember(&set,SIGINT))
       printf("SIGINT\n");
     if(sigismember(&set,SIGTSTP))
       printf("SIGTSTP\n");

     if(sigismember(&set,SIGTERM))
       printf("SIGTERM\n");

     printf("check set end\n\n");
}

