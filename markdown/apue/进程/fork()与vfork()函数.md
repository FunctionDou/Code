﻿# fork()与vfork()函数

标签 : 进程控制

---
大部分来自书上的知识, 然后在网上有些查了掉, 自己再整理了下, 相当于复习, 也写了点自己的理解,不多.

### 进程标识符
进程的ID总是唯一的， 所以我们可以将其用作其他标识符保证其唯一性。即，我们在获取进程的ID，也就能够确定其进行的任务， 可以查看进程，也可以杀死进程。（杀掉僵尸进程）
当一个进程终止的时候， ID也就被系统收回， 可以用ID代表其他的进程（ 其ID就成了复用的候选者）。 但是，系统采用延迟复用算法， 新建的进程不会立刻使用刚终止的进程的ID， 这是为了防止新进程被误认为使用同一ID的某个已终止的先前进程。
> 1. 交换进程(ID=0) : ID为0的进程通常是调度进程(也就是被称为交换进程).该进程是内核的一部分, 它不执行磁盘上的程序, 因此也被称为系统进程.
> 2. init进程(ID=1) : init进程是程序自举过程结束由内核调用. 它一般读取与系统初始化文件有关, 并把系统引导到一个状态, 所以他不会终止.(它是所有成为孤儿进程的父进程)
> 3. 守护进程(ID=2) : 此进程负责支持虚拟存储系统的分页操作.

- - -
### fork()函数

> **pid_t fork(void)**

> 1.返回值:
>: 子进程返回0
>: 父进程返回子进程的ID
>: 错误,返回-1

> 2.没有固定的先后顺序, 可能先执行父进程,也可能先执行子进程. 这个要看系统的调度.
> 3.子进程会复制父进程的数据空间, 堆栈段. 但是父子进程都共用一个正文段.
>: fork()后面接一个exec(), 将采用写时复用. **段只读** 即， 子进程不会复制父进程的所有能复制的数据，而是与父进程共用一个副本数据， 当要修改某段数据的时候才复制这个数据段， 提高了效率。

> 4.父进程打开文件描述符也会被子进程复制, 他们共享一个文件表项
>: 子父进程处理同一个文件就像是原子操作, 同一时刻只能处理一个相同部分(即, 不能同时修改同一个地方)

不管使用哪种线程库， fork()创建的子进程只保留调用线程的副本。

```
/*************************************************************************
    > File Name: fork__缓冲.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月31日 星期三 17时36分30秒
    
     strlen()与sizeof()的区别:
         strlen() : 采用函数进行遍历得到长度, 并且不包含NULL
         sizeof() : 调用的是缓冲区已知字符串的初始化, 长度是固定的, 但是要包含NULL

 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int num = 10;
char ch[] = "write :";

int main(void)
{
    int var;
    var = 0;
    pid_t pid;

    if(write(STDOUT_FILENO, ch, sizeof(ch) -1) != sizeof(ch) - 1)
        err_sys("write error");

    printf("printf\n");

    if((pid = fork()) < 0)
        err_sys("fork error");
    else if(pid == 0)
    {
        var++;
        num++;
    }
    else    
        sleep(2);
    printf("num = %d\nvar = %d\n", num, var);

    exit(0);
}

/*    样例 : ./a.out 
write :printf 
num = 11
var = 1
num = 10
var = 0

[root@192 Process]# ./a.out > temp
[root@192 Process]# cat temp 
write :printf 
num = 11
var = 1
printf
num = 10
var = 0
*/
```
在此可以看出, 
: write两次不同的测验都只输出了一次,printf输出的次数不同.
1. 采用./a.out输出是因为此时缓冲采用的是行缓冲, 所以当输出printf后exit(0)会将其缓冲冲洗, 而重定向文本就是采用的全缓冲, 所以printf没有被冲洗掉.
2. write()函数是不带缓冲的, 所以只会输出一次
: 子父进程的参数不一样, 因为子进程拷贝了一份数据段, 所以数据也就不同是应当的.


同理, 我们也可以修改一下代码就可以看到不同的结果, 由此来验证exit(), _exit()的不同, 虽然之前说过, exit()会执行清理操作, 在进入内核, _exit()直接进入内核.
```
/*****************  第一次修改  ******************/
// 删除exit(0), 加上
_exit(0);

/*****************  第二次修改  ******************/
// 删除exit(0), 加上
if(pid == 0)
    exit(0);
else
    _exit(0);
```


- - -
### vfork()函数

**vfork()函数用于创建一个新的进程,新进程的目地是exec一个新的程序.**
>* vfork()调用时与父进程共用同一个段, 修改数据会直接影响到父进程的数据, 段将不是只读;
>* vfork()当调用exec()函数时, 将创造新的进程, 不再共享段. 相当于写时复制
>* vfork()函数讲明确先进行子进程, 在进行父进程, 不会象fork()函数不确定(fork()要看系统调度)

```
/*************************************************************************
    > File Name: vfork__缓冲.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月31日 星期三 20时03分55秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int val = 10;

int main()
{
	int num;
	num = 0;
	pid_t pid;

	printf("vfork:\n");
	if((pid = vfork()) < 0)
		err_sys("vfork error");
	else if(pid == 0)
	{
		val++;
		num++;
		_exit(0);
	}
	
	printf("pid = %d\nval = %d\nnum = %d\n", pid, val, num);
			

	exit(0);
}
/*    样例
 *
[root@192 Process]# ./a.out
vfork:
pid = 14178
val = 11
num = 1

[root@192 Process]# ./a.out > tep
[root@192 Process]# cat tep
vfork:
pid = 14197
val = 11
num = 1
*/
```

可以看出来, 在子进程修改的数据影响到了父进程的数据, 两个都是共用段的(没有调用嗯exec时)
当将_exit()改为exit()结果也不会变, 因为exit()会冲洗缓冲, 所以还是只有一句vfork.
(可做修改进行验证)
```
/***********************  修改1  *******************************/

  // 删除_exit(0);加上
  exit(0);

/************************  修改2  *******************************/

  // 删除_exit(0)

```


























