#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    /*
    头文件：#include <unistd.h>
    定义函数：int truncate(const char * path, off_t length);

    truncate()会将参数path 指定的文件大小改为参数length 指定的大小. 如果原来的文件大小比参数length 大, 则超过的部分会被删去.
    */

    truncate(argv[1], 5);
    

    exit(EXIT_SUCCESS);
}