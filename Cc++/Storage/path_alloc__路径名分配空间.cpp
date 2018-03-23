/*************************************************************************
    > File Name: path_alloc__路径名分配空间.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月25日 星期四 23时41分21秒

	暂时没有用过, 不怎么理解, 以后再看

 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <errno.h>
#include <limits.h>

#ifdef PATH_MAX
static long pathmax = PATH_MAX;
#else
static long pathmax = 0;
#endif

static long posix_version = 0;
static long xsi_version = 0;

#define PATH_MAX_GUESS 1024

char * path_alloc(size_t * sizep)
{
	char *ptr;
	size_t size;

	if(posix_version == 0)
		posix_version = sysconf(_SC_VERSION);

	if(xsi_version == 0)
		xsi_version = sysconf (_SC_XOPEN_VERSION);

	if (pathmax == 0)
	{
		errno = 0;

		if((pathmax = pathconf("/", _PC_PATH_MAX)) < 0)
			if(errno == 0)
				pathmax = PATH_MAX_GUESS;
			else
				err_sys("pathconf error for _PC_PATH_MAX");
		else
			pathmax++;

	}


	if((posix_version < 200112L) && (xsi_version < 4))
		size = pathmax + 1;
	else
		size = pathmax;

	if ((ptr = malloc(size)) == NULL)
		err_sys("malloc error for pathname");

	if(sizep != NULL)
		*sizep = size;

	return ptr;
}
