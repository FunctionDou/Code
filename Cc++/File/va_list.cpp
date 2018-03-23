/************************************************************************
    > File Name: va_list.cpp
    > Author: Function_Dou
    > Created Time: 2018年01月18日 星期四 12时44分31秒

    VA_LIST 是在C语言中解决变参问题的一组宏，变参问题是指参数的个数不定，可以是传入一个参数也可以是多个;可变参数中的每个参数的类型可以不同,也可以相同;可变参数的每个参数并没有实际的名称与之相对应，用起来是很灵活

    VA_LIST的用法：
       （1）首先在函数里定义一具VA_LIST型的变量，这个变量是指向参数的指针；
       （2）然后用VA_START宏初始化变量刚定义的VA_LIST变量；
       （3）然后用VA_ARG返回可变的参数，VA_ARG的第二个参数是你要返回的参数的类型（如果函数有多个可变参数的，依次调用VA_ARG获取各个参数）；
       （4）最后用VA_END宏结束可变参数的获取。

****************************************************************
    #define _INTSIZEOF(n)   ((sizeof(n)+sizeof(int)-1)&~(sizeof(int) - 1) )
    目的是将求出n对齐后的字节数，从这里来看应该是按照4字节的整数倍来对齐

    va_list ap ;  定义一个va_list变量ap

    va_start(ap,v) ；执行ap = (va_list)&v + _INTSIZEOF(v)，ap指向参数v之后的那个参数的地址，即 ap指向第一个可变参数在堆栈的地址。

    va_arg(ap,t) ， ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )取出当前ap指针所指的值，并使ap指向下一个参数。
    实现将ap指向的参数取出赋给t，并且让p指向下一个可变参数 具体是先(ap += _INTSIZEOF(t)) 之后用 (*( t *)(ap-_INTSIZEOF(t)))，设计的也很巧.

    va_end(ap) ; 清空va_list ap。
    #define va_end(ap) ( ap = (va_list)0 )
    最后将ap指针设置为空。

typedef char *  va_list;  va_list 就是char *类型的指针。
 ************************************************************************/


// 打印字符串

#include <stdarg.h>
#include <stdio.h>
#include "apue.h"

void Function(char *str, ...)
{
    char *str1;

    str1 = str;
    va_list v1;
    va_start(v1, str);
    while(str1)
    {
        printf("%s\t", str1);
        str1 = va_arg(v1, char *);

    }

    va_end(v1);
}


int main()
{
    Function ("one", "two", "three", NULL);



    exit(0);
}






/*
#include <stdio.h>
#include <stdarg.h>
#include "apue.h"

void ar_cnt(int cnt,...);
void ar_cst(char const *s,...);

int main()
{
    //int in_size = _INTSIZEOF(int);
    int in_size = 4;

    printf("int_size=%d\n",in_size);
    ar_cnt(5,1,2,3,4);

    return 0;
}

void ar_cnt(int cnt,...)
{
    int value1=0;
    int i=0;

    va_list arg_ptr;
    va_start(arg_ptr,cnt);

    for(i=0;i<cnt;i++)
    {
        value1=va_arg(arg_ptr,int);
        printf("posation %d=%d\n",value1,i+1);
    }

    va_end(arg_ptr);
}
*/
