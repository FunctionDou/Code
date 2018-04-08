#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

const int SIZE = 1024;

int main(int argc, char *argv[])
{
    char path[SIZE];
    getcwd(path, sizeof(path));

    if(argv[1][0] == '.' && argv[1][1] == '.')
    {
        char *s = strrchr(path, '/');
        s[0] = '\0';
    }

    


    exit(EXIT_SUCCESS);
}