#  exec函数和system()函数

标签（空格分隔）： 进程控制

---
关于这一章基本是按照书上的知识写的， 虽然不明白查了一些资料， 但是后面明白了觉得还是书上写的好， 也就没有自己太多的想法，只是整理加深印象而已， 如果只是寻找理解的朋友， 这就不是很有用了。


> 进程掉用exec时， 该进程执行的程序会完全替换成新的程序， 也就是直接从main函数开始重新执行。
> exec调用不会创建新的进程， 所以也就不会有新的ID产生。
> exec只是用磁盘上新程序替换了现在进程的堆栈, 数据段, 正文段.

**fork()创造新的进程, exec创造新的程序, exit和wait返回进程终止和等待终止.**
**进程中每个打开文件描述符都应该在exec执行前进行关闭, 否则执行exec后这些文件描述符将继续打开而不会关闭**


我觉得着这函数还是后面要用, 自己也想加深一下理解, 所以写了出来, 不想看闲麻烦的也可以跳过
|\#include < unistd.h>|
|:--|
| int execl(const char *\*pathname*, const char *arg0, ... /\* (char*) 0 */)
| int execv(const char *\*pathname*, char *const argv[]);
| int execle(const char *\*pathname*, const char *arg0, ... /\* (char *) 0, char *const envp[] */
| int execve(const char *\*pathname*, char *const argv[], char *const envp[]);
| int execlp(const char *\*filename*, const char *arg0, ... /\* (char *)0 */);
| int execvp(const char *\*filename*, char *const argv[]);
| int fexecve(int *fd*, char *const argv[], char *const envp[]);

>1.  **错误, 返回 -1; 成功, 不返回** 
>2.  前四个取路径名为参数。execlp()和execvp()取文件描述符为参数， 但是如果filename包含 “/”就取路径名， 否则按照PATH环境变量搜寻可执行文件（一般是shell文件）。
>3. execl(), execle(), execlp()函数最后参数以 “NULL”结尾。

```c++
/*************************************************************************
    > File Name: environ_打印环境表参数.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月01日 星期四 21时58分25秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int main(int argc, char *argv[])
{
	int i;
	char **ptr;
	extern char **environ;

	for(i = 0; i < argc; i++)
		printf("argv[%d] : %s\n", i, environ[i]);

    // 输出所有的环境变量
	for(ptr = environ; *ptr != 0; ptr++)
		printf("%s\n", *ptr);


	exit(0);
}

```

- - -
### 解释器

解释器就是#! 后面可执行的程序。
对于解释器，exec族函数是这样做的(以execl为例)，如果path是指向了一个脚本，脚本的第一行以#!开头，则：以#!后面的字符串为命令，后面加上execl参数列表中指定的参数列表，这样形成了新的程序执行。

如何理解解释器呢， 我看了些资料按照自己的思路整理了下。
先创建了一个cpp文件
```c++
/*************************************************************************
    > File Name: jieshiqi.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月01日 星期四 22时18分59秒
 ************************************************************************/

#include <stdio.h>

int main(int argc, char *argv[])
{
	int i;
	for(i = 0; i < argc; i++)
		printf("argv[%d] : %s\n", i, argv[i]);

	return 0;
}
```

又写了一个脚本
```sh
#!/home/RPZ/Code/Cc++/Process/a.out Function

# 这是我jieshiqi.cpp生成的a.out的位置
```
当然权限要设置够
```
-rwxr-xr-x. 1 root root   44 2月   1 22:21 JSQ
```
最后运行JSQ, 结果就出来了
```c++
[root@192 Process]# ./JSQ 
argv[0] : /home/RPZ/Code/Cc++/Process/a.out
argv[1] : Function
argv[2] : ./JSQ
```
>1. 开始时讲到的，exec族函数的处理是把#!后面的字符串为命令，后面加上execl参数列表中指定的参数列表，这样形成了新的程序执行
>2. execl把命令的结果是这样执行的/home/RPZ/Code/Cc++/Process/JSQ的内容是#!/home/RPZ/Code/Cc++/Process/a.out Function，则#!后面的字符串"/home/RPZ/Code/Cc++/Process/a.out"加上命令参数列表："Function" "自身的文件./JSQ(也就是exec族传进来的第一参数)" 就形成了新的程序行：/home/RPZ/Code/Cc++/Process/a.out Function ./JSQ

还是不太清楚可以看下面的地址里讲的， 只是排面有点不好看而已
http://blog.csdn.net/yylklshmyt20090217/article/details/4217616

- - -
### system() 函数

```c++
#include <stdlib.h>

int system("cosnt char *cmdstring);
```

- 关于返回值

> system()在实现中调用了fork(), waitpid(), exec()函数
>1. fork失败或者waitpid返回除EINTR之外的出错, 则system返回 -1, 并设置errno已指示错误类型
>2. exec失败, 返回值如同shell执行exit一样
>3. 3个函数都成功了, system返回值是shell的终止状态

```c++
/*************************************************************************
    > File Name: system_函数的实现.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月01日 星期四 22时43分18秒
 ************************************************************************/

#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include "apue.h"

int system(const char *cmdstring)
{
	pid_t pid;
	int status;

	if(cmdstring == NULL)
		return 1;

	if((pid = fork()) < 0) 
		status = -1;
	else if(pid == 0)
	{
		// shell 的-c选项是告诉shell程序读取下一个命令行参数(这里是cmdstring)作为命令输入(而不是从标准输入或者指定文件读命令);
		execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
		_exit(0);
	}
	else
	  while(waitpid(pid, &status, 0) < 0)
			if(errno != EINTR)
			{
				status = -1;
				break;
			}

	return status;
}

void
pr_exit(int status)
{
  if (WIFEXITED(status))
    printf("normal termination, exit status = %d\n",
        WEXITSTATUS(status));
  else if (WIFSIGNALED(status))
    printf("abnormal termination, signal number = %d%s\n",
        WTERMSIG(status),
#ifdef WCOREDUMP
        WCOREDUMP(status) ? " (core file generated)" : ""
#else
        ""
#endif
        );
  else if (WIFSTOPPED(status))
    printf("child stopped, signal number = %d\n", WSTOPSIG(status));
}

int main(void)
{
	int status;

	if((status = system("date")) < 0)
		err_sys("system() error");

	pr_exit(status);

	if((status = system("no file")) < 0)
		err_sys("system() error");

	pr_exit(status);

	if((status = system("who; exit 44")) < 0)
		err_sys("system() error");

	pr_exit(status);



	exit(0);
}

/*     样列
 *
 [root@192 Process]# ./a.out 
2018年 02月 01日 星期四 22:57:00 CST
normal termination, exit status = 0
sh: no: 未找到命令
normal termination, exit status = 127
root     :2           2018-02-01 14:30 (:2)
root     pts/0        2018-02-01 20:58 (:2)
root     pts/1        2018-02-01 21:40 (:2)
root     pts/2        2018-02-01 21:56 (:2)
normal termination, exit status = 44

*/
```
**如果在调用system就已经有了子进程, 那么就将出问题.**

- - -
### nice()值的效果

这里具体只写了一个程序, 将书上的那个程序进行了解释
```c++
/*************************************************************************
    > File Name: nice_调度.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月02日 星期五 11时53分18秒

***************************  timeval  ***********************************

	struct timeval
	{
		long tv_sec;  //秒
		long tv_usec;  //微秒
	};

	struct timezone
	{
		int tz_minuteswest;  //和Greenwich 时间差了多少分钟
	    int tz_dsttime;  //日光节约时间的状态
	};

***************************  gettimeofday *******************************

	int gettimeofday (struct timeval * tv, struct timezone * tz);

	gettimeofday 会把目前的时间放入tv, 结构中, 当地时区的信息则放入tz结构中.

	返回值：成功则返回0，失败返回－1，错误代码存于errno。

***************************  nice  **************************************

	int nice(int inc);

	nice()用来改变进程的进程执行优先顺序. 参数inc 数值越大则优先顺序排在越后面, 即表示进程执行会越慢. 只有超级用户才能使用负的inc 值, 代表优先顺序排在前面, 进程执行会较快.

	如果执行成功则返回0, 否则返回-1, 失败原因存于errno 中.


****************************  strtol  ***********************************

	long int strtol (const char* str, char** endptr, int base);
	
	strtol() 函数用来将字符串转换为长整型数(long)

	str 为要转换的字符串，endstr 为第一个不能转换的字符的指针，base 为字符串 str 所采用的进制。

	当 base 的值为 0 时，默认采用 10 进制转换，
	若endptr 不为NULL，则会将遇到的不符合条件而终止的字符指针由 endptr 传回；若 endptr 为 NULL，则表示该参数无效，或不使用该参数

******************************  setbuf  *********************************

	void setbuf(FILE *stream, char *buf)

	使得文件打开后, 用户可建立自己设置的缓冲区, 而不使用fopen()打开文件设定的默认缓冲区大小

	buf = 0. 文件I/O不带缓冲

 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <errno.h>
#include <sys/time.h>

// 不同系统的操作
#if defined(MACOS)
#include <sys/limits.h>
#elif defined(SOLARIS)
#include <limits.h>
#elif defined(BSD)
#include <sys/param.h>
#endif


unsigned long long count;
struct timeval end;

void checktime(char *str)
{
	struct timeval tv;
	
	// 获取当前的时间放入tv里面.
	gettimeofday(&tv, NULL);
	if(tv.tv_sec >= end.tv_sec && tv.tv_usec >= end.tv_usec)
	{
		printf("%s %lld\n", str, count);
		exit(0);
	}
}

int main(int argc, char *argv[])
{
	pid_t pid;
	char *s;
	int nzero, ret;
	int adj = 0;

	setbuf(stdout, NULL);

#if defined(NZERO)
	nzero = NZERO;
#elif defined(_SC_NZERO)
	nzero = sysconf(_SC_NZERO);
#else
#error NZERO undefined
#endif

	printf("NZERO = %d\n", nzero);
	if(argc == 2)
		// 转换为对应的整形
		adj = strtol(argv[1], NULL, 0);
	// 获得当前的时间
	gettimeofday(&end, NULL);
	end.tv_sec += 10;
		
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
	{
		s = "child";
		printf("current nice value in child is %d, adjusting by %d\n", nice(0) + nzero, adj);
		errno = 0;

		if((ret = nice(adj)) == -1 && errno != 0)
			err_sys("child set scheduling priority");
		printf("now child nice value in parent is %d\n", nice(0) + nzero);
	}

	else
	{
		s = "parent";
		printf("current nice value in parent is %d\n", nice(0) + nzero);
	}
	for(; ; )
	{
		if(++count == 0)
			err_quit("%s counter wrap", s);
		checktime(s);
	}
}

```

