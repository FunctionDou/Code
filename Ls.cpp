#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>


void Mode(mode_t, char *);
void Dir(mode_t, char *);

void Dir(mode_t mode, char *s)
{

    int i = 0;
    if (S_ISREG(mode))
        s[i] = '-';
    else if (S_ISDIR(mode))
        s[i] = 'd';
    else if (S_ISCHR(mode))
        s[i] = 'c';
    else if (S_ISBLK(mode))
        s[i] = 'b';
    else if (S_ISLNK(mode))
        s[i] = 'l';
    else if (S_ISSOCK(mode))
        s[i] = 's';
}

void Mode(mode_t mode, char *s)
{
    int i = 1;
    if (mode & S_IRUSR)
        s[i++] = 'r';
    else
        s[i++] = '-';

    if (mode & S_IWUSR)
        s[i++] = 'w';
    else
        s[i++] = '-';

    if (mode & S_IXUSR)
        s[i++] = 'x';
    else
        s[i++] = '-';

    if (mode & S_IRGRP)
        s[i++] = 'r';
    else
        s[i++] = '-';

    if (mode & S_IWGRP)
        s[i++] = 'w';
    else
        s[i++] = '-';

    if (mode & S_IXGRP)
        s[i++] = 'x';
    else
        s[i++] = '-';

    if (mode & S_IROTH)
        s[i++] = 'r';
    else
        s[i++] = '-';

    if (mode & S_IWOTH)
        s[i++] = 'w';
    else
        s[i++] = '-';

    if (mode & S_IXOTH)
        s[i++] = 'x';
    else
        s[i++] = '-';

    s[i] = '\0';
}

int main(int argc, char *argv[])
{
    struct stat st;
    struct dirent *dirt;
    DIR *dir;
    char *tim;
    char buf[15];
    char *pwd = NULL;
    buf[0] = '-';

    chdir(argv[1]);
    pwd = getcwd(pwd, 0);
    printf("%s\n", pwd);
    
    if ((dir = opendir(pwd)) == NULL)
        ;
    while ((dirt = readdir(dir)) != NULL)
    {
        //Dir(dirt->d_type, buf);
        //Mode(dirt->d_type, buf);
        if ((lstat(dirt->d_name, &st)) != -1)
        {
            //printf("%d %d ", dirt->d_type, st.st_mode);
            Dir(st.st_mode, buf);
            Mode(st.st_mode, buf);
            tim = ctime(&st.st_atime);
            tim[strlen(tim) - 1] = '\0';
            printf("%s %d %s %s  %d %s %s\n", buf, (int)st.st_nlink, getgrgid(st.st_gid)->gr_name, getpwuid(st.st_uid)->pw_name, st.st_size, tim, dirt->d_name);
        }
        //printf("%s\n", dirt->d_name);
    }
    closedir(dir);

    if (dirt == NULL)
    {
        stat(argv[0], &st);
        Dir(st.st_mode, buf);
        Mode(st.st_mode, buf);
        printf("%s %d %d\n", buf, st.st_uid, st.st_gid);
    }
    exit(0);
}