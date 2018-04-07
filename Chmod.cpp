#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "less code\n");
        exit(0);
    }
    int num_8 = strtol(argv[1], NULL, 8);
    chmod(argv[2], num_8);

    exit(EXIT_SUCCESS);
}