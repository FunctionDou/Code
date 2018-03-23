/*************************************************************************
    > File Name: ls.cpp
    > Author: Function_Dou
    > Created Time: 2018年01月18日 星期四 11时00分55秒
    > 第一次修改 : 增加了err_sys, err_quit的注释, 来源.  18.1.18/ 13:11

***********************************************************************
    > agrc : 代表传入的参数的个数
    > agrv[] : 代表传入的变量, agrv[0]代表文件名

*************************************************************************
    > 函数 DIR *opendir(const char *pathname)，即打开文件目录返,回的就是指向DIR结构体的指针，而该指针由以下几个函数使用:
    
    // opendir()用来打开参数name 指定的目录, 并返回DIR*形态的目录流, 和open()类似, 接下来对目录的读取和搜索都要使用此返回值.
    // 成功则返回DIR* 型态的目录流, 打开失败则返回NULL.
    DIR * opendir(const char * pathname)          

    struct dirent *readdir(DIR *dp);        // 返回参数dir 目录流的下个目录进入点

    DIR *opendir(const char *pathname);     // 第一步, 打开文件, 执行初始化操作

    void rewinddir(DIR *dp);                // 返回目录中的第一个目录项

    int closedir(DIR *dp);                  // 最后一步, 关闭文件

    long telldir(DIR *dp);

    void seekdir(DIR *dp,long loc);

    dirent不仅仅指向目录，还指向目录中的具体文件，readdir函数同样也读取目录下的文件

*****************************  DIR  dirent  *************************************
    
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

 ************************************************************************/


#include "apue.h"
#include <dirent.h>

int main(int argc, char *argv[])
{
    DIR *dp;
    struct dirent *dirp;

    // err_sys, err_quit : 是apue.h里面的自定义的, 原型是用va_list(), va_start(), va_end()写的, 了解可看文件va_list.cpp
    if (argc != 2)
        err_quit("usage: ls directory_name");

    if ((dp = opendir(argv[1])) == NULL)
        err_sys("can't open %s", argv[1]);

    while((dirp = readdir(dp)) != NULL)
        printf("%s\n", dirp->d_name);

    closedir(dp);
    exit(0);
}
