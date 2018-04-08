#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>

static jmp_buf jmp_i;
static jmp_buf jmp_j;
static int i = 1;
static int j = 1;

int Fun1();
void Fun2();

int Fun1()
{
    if (setjmp(jmp_i) < 9)
    {
        if (setjmp(jmp_j) < 9)
            printf("%d*%d=%d\t", i, j, i * j);
        if (j == i)
            printf("\n");
        //sleep(1);
        Fun2();
    }
}

void Fun2()
{
    if(j < i)
        longjmp(jmp_j, j++);
    if(i < 9)
    {
        j = 1;
        longjmp(jmp_i, i++);
    }
}

int main(void)
{
    Fun1();

    exit(EXIT_SUCCESS);
}