/*************************************************************************
    > File Name: time_strftime_系统时间转换.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年01月27日 星期六 11时40分45秒

*********************************  time_t time  **************************
	time_t是定义在time.h中的一个类型，表示一个日历时间，也就是从1970年1月1日0时0分0秒到此时的秒数，原型是：
	
	typedef long time_t; 

	函数time可以获取当前日历时间时间，time的定义：
	
	time_t time(time_t *)
 

*******************************  struct tm  ******************************
	time_t只是一个长整型，不符合我们的使用习惯，需要转换成本地时间，就要用到tm结构，time.h中结构tm的原型是：

	struct tm 
	{
          int tm_sec;        秒 – 取值区间为[0,59] 
          int tm_min;        分 - 取值区间为[0,59] 
          int tm_hour;       时 - 取值区间为[0,23] 
          int tm_mday;       一个月中的日期 - 取值区间为[1,31] 
          int tm_mon;		 月份（从一月开始，0代表一月） - 取值区间为[0,11] 
          int tm_year;       年份，其值等于实际年份减去1900 
          int tm_wday;       星期 – 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 
          int tm_yday;       从每年的1月1日开始的天数 – 取值区间为[0,365]，其中0代表1月1日，1代表1月2日

	 };

************************  localtime ctime  asctime   *******************

	用localtime获取当前系统时间，该函数将一个time_t时间转换成tm结构表示的时间，函数原型：
	
	struct tm * localtime(const time_t *)

	char * asctime(const struct tm *)

	char * ctime(const time_t *)

	这两个函数返回值都是一个表示时间的字符串，区别在于传入的参数不同。


*******************************  strftime  ******************************

	size_t strftime(char *strDest, size_t maxsize, const char *format, const struct tm *timeptr);

	参数说明 : 
	char *strDest：用来存放格式化后的字符串缓存，
	size_t maxsize：指定最多可以输出的字符数，
	const char *format：格式化字符串，
	const struct tm *timeptr：要转换的时间。


	可使用的格式化字符串：
	%a 星期几的简写
	%A 星期几的全称
	%b月分的简写
	%B月份的全称
	%c 标准的日期的时间串
	%C 年份的后两位数字
	%d 十进制表示的每月的第几天
	%D 月/天/年
	%e 在两字符域中，十进制表示的每月的第几天
	%F 年-月-日
	%g 年份的后两位数字，使用基于周的年
	%G 年分，使用基于周的年
	%h 简写的月份名
	%H 24小时制的小时
	%I 12小时制的小时
	%j 十进制表示的每年的第几天
	%m 十进制表示的月份
	%M 十时制表示的分钟数
	%n 新行符
	%p 本地的AM或PM的等价显示
	%r 12小时的时间
	%R 显示小时和分钟：hh:mm
	%S 十进制的秒数
	%t 水平制表符
	%T 显示时分秒：hh:mm:ss
	%u 每周的第几天，星期一为第一天 （值从0到6，星期一为0）
	%U 第年的第几周，把星期日做为第一天（值从0到53）
	%V 每年的第几周，使用基于周的年
	%w十进制表示的星期几（值从0到6，星期天为0）
	%W 每年的第几周，把星期一做为第一天（值从0到53）
	%x标准的日期串
	%X 标准的时间串
	%y 不带世纪的十进制年份（值从0到99）
	%Y 带世纪部分的十进制年份
	%z，%Z 时区名称，如果不能得到时区名称则返回空字符。
	%% 百分号

 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int MAX = 100;
struct timeval tt;

int main(void)
{
	time_t t;
	struct tm *tmp;


	char *buf1;
	char *buf2;
	
	time(&t);
	tmp = localtime(&t);

	buf1 = asctime(tmp);
	buf2 = ctime(&t);

	printf("%ld\n", t);
	printf("%s\n", buf1);
	printf("%s\n", buf2);
	
	printf("%d-%d-%d / %d:%d:%d\n", tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday, tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	char buf[MAX];
	if(strftime(buf, sizeof(buf), "time is : %Y-%m-%d / %H:%M:%S", tmp) != 0)
		printf("%s\n", buf);
	else
		printf("strftime error");


	exit(0);
}
