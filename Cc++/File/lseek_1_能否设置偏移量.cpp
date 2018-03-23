/*************************************************************************
    > File Name: lseek_1_能否设置偏移量.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月25日 星期四 23时57分02秒


	头文件：#include <sys/types.h>    #include <unistd.h>

	定义函数：off_t lseek(int fildes, off_t offset, int whence);


    SEEK_SET 参数offset 即为新的读写位置.
    SEEK_CUR 以目前的读写位置往后增加offset 个位移量.
    SEEK_END 将读写位置指向文件尾后再增加offset 个位移量. 当whence 值为SEEK_CUR 或
    SEEK_END 时, 参数offet 允许负值的出现

	1) 欲将读写位置移到文件开头时:lseek(int fildes, 0, SEEK_SET);
	2) 欲将读写位置移到文件尾时:lseek(int fildes, 0, SEEK_END);
	3) 想要取得目前文件位置时:lseek(int fildes, 0, SEEK_CUR);
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int main(void)
{
	if(lseek(STDIN_FILENO, 0, SEEK_CUR) == -1)
		printf("cannot seek\n");
	else
		printf("seek OK\n");
	

	exit (0);
}
