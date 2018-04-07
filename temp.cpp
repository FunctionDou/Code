// getopt() 所设置的全局变量包括：

// optarg——指向当前选项参数（如果有）的指针。
// optind——再次调用 getopt() 时的下一个 argv 指针的索引。
// optopt——最后一个已知选项。

/*
#include <getopt.h>
int getopt(int argc, char * const argv[], const char * optstring);
int getopt_long(int argc, char *const argv[], const char *optstring, const struct option *longopts, int *longindex);
int getopt_long_only(int argc, char *const argv[], const char *optstring, const struct option *longopts, int *longindex);
struct option
{
    const char *name;
    int         has_arg;
    int        *flag;
    int         val;
};
has_arg
是：no_argument（或0）如果选项没有参数; required_argument（或1）如果选项需要参数; 或 optional_argument（或2）如果该选项采用可选参数。
*/


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

int a = 1;
struct option opt[] =
    {
        {"help", 0, NULL, 'h'},
        {"version", 1, &a, 'v'},
        {"ww", 2, NULL, 'w'}};
int s;

int main(int argc, char *argv[])
{
    int ch;

    //  getopt_long_only 时, - 和 --都可以作用于长选项. getopt_long 时, 只有 --可以作用于长选项.

    // if((ch = getopt_long_only(argc, argv, "hvw", opt, &s)) != -1)
    // {
    //     switch(ch)
    //     {
    //         case 'h':
    //             printf("help%d : %s\n", s, optarg);
    //             break;
    //         case 'v':
    //             printf("version%d : %s\n", s, optarg);
    //             break;
    //         case 'w':
    //             printf("ww%d : %s\n", s, optarg);
    //             break;
    //     }
    // }

    // if((ch = getopt_long(argc, argv, "abcd:", opt, &s)) != -1)
    // {
    //     switch(ch)
    //     {
    //         case 'h':
    //             printf("help\n");
    //             break;
    //         case 0:
    //             printf("version%d : %s\n", s, optarg);
    //             break;
    //         case 'w':
    //             printf("ww\n");
    //             break;
    //     }
    // }

    // while((ch = getopt(argc, argv, "abcd:e:f::")) != -1)
    // {
    //     switch(ch)
    //     {
    //         case 'a':
    //             printf("aaaa%s\n", optarg);
    //             break;
    //         case 'b':
    //             printf("bbbb%s\n", optarg);
    //             break;
    //         case 'c':
    //             printf("cccc%s\n", optarg);
    //             break;
    //         case 'd':
    //             printf("dddd%s\n", optarg);
    //             break;
    //         case 'e':
    //             printf("eeee%s\n", optarg);
    //             break;
    //         case 'f':
    //             printf("ffff%n", optarg);
    //             break;
    //         default:
    //             printf("nonono");
    //             //break;

    //     }

/*
EXIT_SUCCESS	程序的成功执行
EXIT_FAILURE	程序的不成功执行
注意
EXIT_SUCCESS 与值零均指示成功的程序执行状态（见 std::exit ），尽管不要求 EXIT_SUCCESS 等于零。
*/
    printf("%d : %d\n", EXIT_SUCCESS, EXIT_FAILURE);
    exit(EXIT_SUCCESS);
}