/*************************************************************************
    > File Name: getID.cpp
    > Author: Function_Dou
    > Created Time: 2018年01月18日 星期四 13时12分59秒
 ************************************************************************/
#include <stdio.h>
//#include <iostream>
#include <unistd.h>
#include "apue.h"
//using namespace std;


int main()
{
    // getpid() : 在头文件unistd.h文件里面
    // 取得进程识别码，许多程序利用取到的此值来建立临时文件，以避免临时文件相同带来的问题
    printf("process ID %ld uid = %d. gid = %d\n", (long)getpid(), getuid(), getgid());

    return 0;
}


/*
样列
process ID 3864 uid = 0. gid = 0
process ID 3999 uid = 0. gid = 0
*/
