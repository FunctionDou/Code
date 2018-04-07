#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "less arg and <pathname>\n");
        exit(0);
    }

    // strtok : 返回分隔符的地址, 并将分隔符置为 \0, 下次使用可以直接用NULL, 没有找到返回NULL.
    // char *s = strtok(argv[1], ":");
    // char *p = strtok(NULL, ":");
    // printf("%s %s", s, p);

    // strstr : 返回字串的第一次出现的位置的地址.
    char *s = strstr(argv[1], ":");
    s[0] = '\0';
    s++;
    printf("%s %s\n", argv[1], s);
    struct passwd *owner = getpwnam(argv[1]);
    struct passwd *group = getpwnam(s);
    chown(argv[2], owner->pw_uid, group->pw_gid);

    exit(EXIT_SUCCESS);
}