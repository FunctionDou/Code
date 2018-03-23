/*************************************************************************
    > File Name: t.cpp
    ed device is claimed by another driver (a> Author: Function_Dou
    > Mail: NOT
    > Created Time: Mon Mar 19 13:05:48 2018
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
	int fd;
	fd = open("re_malloc.cpp", 1 | 0, 0);
	printf("open_fd = %d\n", fd);

    close(fd);
    printf("close_opend_fd = %d\n", fd);


    int fd1;
    char c, c1;
    fd = open("re_malloc.cpp", O_RDONLY, 0);
    fd1 = open("re_maloc.cpp", O_RDONLY, 0);
    read(fd, &c, 1);
    read(fd1, &c1, 1);
    printf("fd = %d\nfd1 = %d\n", c, c1);
    close(fd);
    close(fd1);
    printf("fd = %d\nfd1 = %d\n", c, c1);

    // int fd2;
    // fd2 = open("re_malloc.cpp", O_RDONLY, 0);
    // if(fork() == 0)
    // {
    //     read(fd2, &c, 1);
    //     exit(0);
    // }
    // // wait(NULL);
    // read(fd2, &c, 1);
    // printf("fd2 = %d\n", c);


	exit(0);
}
