/*************************************************************************
    > File Name: S_IS__文件类型.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月26日 星期五 21时12分55秒


	dirent结构体存储的关于文件的信息很少，所以dirent同样也是起着一个索引的作用，如果想获得类似ls -l那种效果的文件信息，必须要靠stat函数了
    eaddir函数读取到的文件名存储在结构体dirent的d_name成员中，而函数

    int stat(const char *file_name, struct stat *buf);

    struct stat
    {

            mode_t     st_mode;       //文件访问权限

            ino_t      st_ino;       //索引节点号

            dev_t      st_dev;        //文件使用的设备号

            dev_t      st_rdev;       //设备文件的设备号

            nlink_t    st_nlink;      //文件的硬连接数

            uid_t      st_uid;        //所有者用户识别号

            gid_t      st_gid;        //组识别号

            off_t      st_size;       //以字节为单位的文件容量

            time_t     st_atime;      //最后一次访问该文件的时间

            time_t     st_mtime;      //最后一次修改该文件的时间

            time_t     st_ctime;      //最后一次改变该文件状态的时间

            blksize_t st_blksize;    //包含该文件的磁盘块的大小

            blkcnt_t   st_blocks;     //该文件所占的磁盘块

     };


	S_ISREG() 　普通文件
　　S_ISDIR()　 目录文件
　　S_ISCHR()　 字符特殊文件
　　S_ISBLK()　 块特殊文件
　　S_ISFIFO()　管道或FIFO
　　S_ISLNK()　 符号链接
　　S_ISSOCK()	套接字

 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int main(int argc,char * argv[])
{
	int i;
	struct stat buf;
	char *ptr;
	
	for(i = 1; i < argc; i++)
	{
		printf("%s: ", argv[i]);
		if(lstat(argv[i], &buf) < 0)
		{

			err_ret ("lstat error");
			continue;
		}

		if(S_ISREG(buf.st_mode))
			ptr = "regular";
		else if (S_ISDIR(buf.st_mode))
			ptr = "directory";
		else if(S_ISCHR(buf.st_mode))
			ptr = "character special";
		else if(S_ISBLK(buf.st_mode))
			ptr = "block special";
		else if(S_ISFIFO(buf.st_mode))
			ptr = "fifo";
		else if(S_ISLNK(buf.st_mode))
			ptr = "symbolic link";
		else if(S_ISSOCK(buf.st_mode))
			ptr = "socket";
		else
			ptr = "** unknwon mode **";

		printf("%s\n", ptr);
	}
	exit(0);
}

