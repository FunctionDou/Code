#include <unistd.h>  
#include <stdlib.h>    
#include <stdio.h>

int main(void)  
{
   char buf[10];
   int n;

   n = read(STDIN_FILENO, buf, 10);
   if (n < 0)
   {
    perror("read STDIN_FILENO");
    exit(1);
   }
   
   write(STDOUT_FILENO, buf, n);
   
   return 0;
}


/*
 
   第一次执行a.out的结果很正常，而第二次执行的过程有点特殊，现在分析一下：

   Shell进程创建a.out进程，a.out进程开始执行，而Shell进程睡眠等待a.out进程退出。

   a.out调用read时睡眠等待，直到终端设备输入了换行符才从read返回，read只读走10个字符，剩下的字符仍然保存在内核的终端设备输入缓冲区中。

   ./a.out
   进程打印并退出，这时Shell进程恢复运行，Shell继续从终端读取用户输入的命令，于是读走了终端设备输入缓冲区中剩下的字符d和换行符，把它当成一条命令解释执行，结果发现执行不了，没有d这个命令
*/ 
