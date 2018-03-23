/*************************************************************************
    > File Name: opendir_readdir_closedir.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年02月03日 星期六 14时13分55秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char *argv[])
{
	if(argc == 1)
	{
		printf("no pathname");
		exit(0);
	}

	int i;
	struct dirent *dp;
	DIR * pathname;
	struct stat statbuf;
	for(i = 1; argv[i] != NULL; i++)
	{
		if((pathname = opendir(argv[i])) == NULL)
		{
			printf("opendir error");
			exit(0);
		}
		while((dp = readdir(pathname)) != NULL)
		{
			if(stat(dp->d_name, &statbuf) < 0)
			{
				printf("stat error");
			}
			printf("d_inc : %ld, d_off = %ld, d_reclen = %d, d_type = %c, d_name = %s\n", dp->d_ino, dp->d_off, dp->d_reclen, dp->d_type, dp->d_name);
			printf("nlink = %ld, uid = %d, gid = %d, atime = %ld, ctime = %ld, mtime = %ld\n\n", statbuf.st_nlink, statbuf.st_uid, statbuf.st_gid, statbuf.st_atim, statbuf.st_ctim, statbuf.st_mtim);
		}

		if(closedir(pathname) != 0)
			 perror("eror : ");
	}

	exit(0);
}
