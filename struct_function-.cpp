/*************************************************************************
    > File Name: struct_function-.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月27日 星期六 12时31分28秒
 ************************************************************************/

#include <stdio.h>

int main(void)
{
	return 0;
}


/*
 ********************  DIR  dirent  ***********************

    // opendir()用来打开参数name 指定的目录, 并返回DIR*形态的目录流, 和open()类似, 接下来对目录的读取和搜索都要使用此返回值.
    // 成功则返回DIR* 型态的目录流, 打开失败则返回NULL.
    DIR * opendir(const char * pathname)          

    struct dirent *readdir(DIR *dp);        // 返回参数dir 目录流的下个目录进入点

    DIR *opendir(const char *pathname);     // 第一步, 打开文件, 执行初始化操作

	 int closedir(DIR *dp);                  // 最后一步, 关闭文件



    
    struct __dirstream   
    {   
        void *__fd;    
        char *__data;    
        int __entry_data;    
        char *__ptr;    
        int __entry_ptr;    
        size_t __allocation;    
        size_t __size;    
        __libc_lock_define (, __lock)    
    };   
  
    typedef struct __dirstream DIR;  
　　
    struct dirent
    {
　　    long d_ino;                 // inode number 索引节点号 
　　
        off_t d_off;                //  offset to this dirent 在目录文件中的偏移 
　　
        unsigned short d_reclen;    //  length of this d_name 文件名长 
　　
        unsigned char d_type;       //  the type of d_name 文件类型 
　　
        char d_name [NAME_MAX+1];   // file name (null-terminated) 文件名，最长255字符 
    }

********************   read   ************************

	#include <unistd>
	
	ssize_t read(int filedes, void *buf, size_t nbytes);
	
	num = read(...);
	num 返回：若成功则返回读到的字节数，若已到文件末尾则返回0，若出错则返回-1
	number：表示系统实际所读取的字符数量。

	filedes：文件描述符
	
	buf:读取数据缓存区

	nbytes:要读取的字节数。


********************  write   **************************

	#include <unistd>

	ssize_t write(int filedes, void *buf, size_t nbytes);
	
	返回：若成功则返回写入的字节数，若出错则返回-1
	
	filedes：文件描述符
	
	buf:待写入数据缓存区
	
	nbytes:要写入的字节数 


****************** getc putc fgets fputs ****************

	fgets(buf, MAXLINE, stdin) != NULL
	fputs(buf, stdout) != EOF
	if(ferror(stdin) < 0)

	(tmp = getc(stdin)) != EOF
	putc(tmp, stdout) == EOF
	if(ferror(stdin) < 0)


*****************************  mkstemp  *********************************
	
	int mkstemp(char *template);
	
	mkstemp函数在系统中以唯一的文件名创建一个文件并打开，而且只有当前用户才能访问这个临时文件，并进行读、写操作
	
	与tmpnam相似，所不同的是我们可以为临时文件名指定一个模板，这样我们就可以更好的控制其位置与名字.
	
	char *mktemp(char *template);
	int mkstemp(char *template);
	
	mktemp函数由指定的模板生成一个唯一的文件名。template必须以6个x开始。mktemp函数使用唯一可用的文件名字符来替换这些x字符。他返回一个指向所生成的字符串的指针，如果不可以生成一个唯一的文件名，则会返回一个null指针。
	mkstemp函数在创建与打开临时文件方面与tmpfile相类似。文件名由与mktemp相同的方式生成的，但是返回的结果是一个打开的，底层文件描述符


***************************  fork  ********************************

	fork调用的一个奇妙之处就是它仅仅被调用一次，却能够返回两次，它可能有三种不同的返回值：
     1）在父进程中，fork返回新创建子进程的进程ID；
     2）在子进程中，fork返回0；
     3）如果出现错误，fork返回一个负值；

	  execlp() : 从PATH 环境变量中查找文件并执行
    execlp() 会从PATH 环境变量所指的目录中查找符合参数file的文件名, 找到后便执行该文件, 然后将第二个以后的参数当做该文件的argv[0]、argv[1]……, 最后一个参数必须用空指针(NULL)作结束。
    如果执行成功则函数不会返回,执行失败则直接返回-1,失败原因存于errno 中

	
************************  strerror()   *****************************
	头文件：#include <string.h>

	定义函数：char * strerror(int errnum);
	
	函数说明：strerror()用来依参数errnum 的错误代码来查询其错误原因的描述字符串, 然后将该字符串指针返回.

	返回值：返回描述错误原因的字符串指针.


************************* perror  *********************************

	头文件：#include <stdio.h>

	函数perror()用于抛出最近的一次系统错误信息

    函数定义 : void perror(char *string);

	string为要输出的错误信息
	perror()用来将上一个函数发生错误的原因输出到标准错误(stderr)。
	参数string所指的字符串会先打印出，后面再加上错误原因字符串，此错误原因依照全局变量errno 的值来决定要输出的字符串。



*********************************  time_t time  **************************

	typedef long time_t;

	time_t time(time_t *)

	struct tm * localtime(const time_t *)

	char * asctime(const struct tm *)

	char * ctime(const time_t *)

	size_t strftime(char *strDest, size_t maxsize, const char *format, const struct tm *timeptr);
		参数说明 :
	char *strDest：用来存放格式化后的字符串缓存，
	size_t maxsize：指定最多可以输出的字符数，
	const char *format：格式化字符串，
	const struct tm *timeptr：要转换的时间。


*****************************  tmp  ************************************

	char *tmpnam(char *str) 生成并返回一个有效的临时文件名，该文件名之前是不存在的。如果 str 为空，则只会返回临时文件名.

	FILE *tmpfile(void) 以二进制更新模式(wb+)创建临时文件。被创建的临时文件会在流关闭的时候或者在程序终止的时候自动删除.

	rewind()函数用于将文件指针重新指向文件的开头，同时清除和文件流相关的错误和eof标记
	void rewind(FILE * stream);


*/
