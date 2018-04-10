# 线程操作(pthread_*)

标签（空格分隔）： 线程

---

**一个进程的所有信息对该进程的的所有线程都是共享的, 包括可执行程序的代码, 程序的全局内存和堆内存, 栈以及文件描述符.**
**所以, 多个线程自动的可以访问相同的存储地址空间和文件描述符**
**但是, 新的线程会清除原线程的信号集**
 
每个线程的ID也是有意义的, 但是仅在它所属的进程上下文才有意义.
 
```c
#include <pthread.h>

// 比较两个线程ID是否相等
// 线程 ID 不一定是一个非负整数,也有可能是一个结构体。所以,要对比两个线程是否相同,必须使用pthread_equal 函数来进行.
int pthread_equal(pthread_t tidl, pthread_t tid2)
// 返回值 : 相等, 返回 !0; 不相等, 返回 0.
```

```c
#include <pthread.h>

pthread_t pthread_self(void)
// 返回线程的ID
```
每个线程只能做被安排了的作业.

```c
#include <pthread.h>

int pthread_create(pthread_t *restrict tidp, const pthread_attr_t *restrict attr, void *(*start_rtn)(void *), void *restrict arg)
// 成功, 返回 0; 失败, 返回错误编码
```

pthread_create 参数 : 
> 
tidp : 指向新线程ID的内存单元
attr : 定制不同属性的线程. (attr = NULL, 默认属性)
start_rtn : 指向线程执行的函数
arg : 函数传入的参数. (arg = NULL, 不传入参数)

```c
/*************************************************************************
    > File Name: create_2_创建线程.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月05日 星期一 19时34分03秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

void print_id(const char *s)
{
	pthread_t tid;
	pid_t pid;

	pid = getpid();
	tid = pthread_self();

	printf("%s pid = %d, tip = %lu\n", s, pid, (unsigned long)tid);
}

void *thread_print(void *)
{
	print_id("new thread :");

	return ((void *)0);
}

int main(void)
{
	pthread_t tid;

	errno = pthread_create(&tid, NULL, thread_print, NULL);
	if(errno != 0)
		fprintf(stderr, "%s", strerror(errno));
	
	// 主程序沉睡 1s, 不让两个程序混乱
	sleep(1);
	print_id("main thread :");

	exit(0);
}
```
运行结果
```c
// 注意的时候, gcc 后面要加上 -pthread, 如 : gcc -pthread 文件名.cpp
[root@localhost Pthread]# gcc create_2_创建线程.cpp -pthread
[root@localhost Pthread]# ./a.out 
new thread : pid = 19673, tip = 140307662010112
main thread : pid = 19673, tip = 140307670284096
```







