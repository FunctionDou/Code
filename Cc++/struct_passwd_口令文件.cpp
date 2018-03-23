/*************************************************************************
    > File Name: struct_passwd_口令文件.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月29日 星期一 13时46分58秒


*****************************  grp group  *******************************
	
	getgrent()用来从组文件（/etc/group）中读取一项组数据，该数据以group结构返回。第一次调用时会取得第一项组数据，之后每调用一次就会返回下一项数据，直到已无任何数据时返回NULL。
	struct group
	{
		char *gr_name;		组名称
		char *gr_passwd;	组密码
		gid_t gr_gid;		组识别码
		char **gr_mem;		组成员账
	}
	返回group结构数据，如果返回NULL则表示已无数据，或有错误发生。getgrent（）在第一次调用时会打开组文件，读取数据完毕后可使用endgrent（）来关闭该组文件

****************************  pwd passwd  *******************************

	struct passwd 
	{  
	    char * pw_name;			Username, POSIX.1 
	    char * pw_passwd;		Password *
	    __uid_t pw_uid;			User ID, POSIX.1 
	    __gid_t pw_gid;			Group ID, POSIX.1 
	    char * pw_gecos;		Real Name or Comment field 
	    char * pw_dir;			Home directory, POSIX.1  
	    char * pw_shell;		Shell Program, POSIX.1 
	};  


	相关函数:
	
	struct passwd * getpwuid(uid_t uid);
	当您知道使用者的uid(user id)时，可以透过getpwuid来得知所有关於该使用者的相关资讯。  

	struct passwd * getpwnam(char * name);  
	当您知道使用者名称时，可以透过getpwnam来得知所有关於该使用者的相关资讯。

	int getpw(uid_t uid, char *buf);  
	当您仅需要取得使用者的密码进行比对时，可以使用getpw。  
 
	void endpwent(void);
	关闭password档案资料流。  
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <stddef.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <sys/stat.h>

// 书上的程序
struct passwd *getpwnam(const char *name)
{
	struct passwd *ptr;

	setpwent();			// 自我保护措施
	while((ptr = getpwent()) != NULL)
		if(strcmp(name, ptr->pw_name) == 0)
			break;

	endpwent();
	return (ptr);
}


// 补充的程序, 以便提高理解passwd
int main(int argc, char * argv[])
{
	int i;

	struct stat statbuf;
	struct passwd *passwdbuf;
	struct group *grp;

	if(argc < 2)
		err_sys("error <pathname>");

	for(i = 0; argv[i] != NULL; i++)
	{
		if(stat(argv[i], &statbuf) == -1)
			err_sys("error %s stat", argv[i]);

		passwdbuf = getpwuid(statbuf.st_uid);
		grp = getgrgid(statbuf.st_gid);

		printf("uis:%s\n", argv[i]);
		printf("name : %s\n", passwdbuf->pw_name);
		printf("uid : %d\n", passwdbuf->pw_uid);
		printf("gid : %d\n", passwdbuf->pw_gid);
		printf("dir ; %s\n", passwdbuf->pw_dir);
		printf("shell : %s\n\n", passwdbuf->pw_shell);
		

		printf("gid:");
		printf("gr_name : %s\n", grp->gr_name);
		printf("gr_passwd : %s\n", grp->gr_passwd);
		printf("gr_gid : %d\n\n\n\n", grp->gr_gid);

		endpwent();
		endgrent();

	}



	exit(0);
}

