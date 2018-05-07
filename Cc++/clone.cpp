#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <sched.h>

const int SIZE = 8192;

//int clone(int (*fun)(void *), void *child_statck, int flags, void *arg);
int Fun(void *)
{
    printf("hello world! %ld\n", (long)getpid());

    exit(0);
}

int main()
{
    void *statck;
    statck = malloc(SIZE);
    
    clone(Fun, (char *)statck + SIZE, CLONE_FILES | CLONE_VM, 0);
    printf("%ld\n", (long)getpid());
    sleep(1);
    exit(EXIT_SUCCESS);
   // do_exit();
}