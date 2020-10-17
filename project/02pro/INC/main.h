#ifndef _MAIN_H_
#define _MAIN_H_
/*----------引入系统相关头文件----------*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>         //引入配置内存相关头文件
#include <string.h>         //为了使用bzero(void *s,int n)
#include <linux/input.h>    //引入linux关于触摸屏的头文件
#include <dirent.h>         //引用目录类函数

/*----------引入自定义头文件----------*/
#include "ILI93xx.h"    //引入LCD的设置
#include "Touch.h"      //引入触摸屏相关设置
#include "File.h"       //引入读取文件头文件

/*----------    宏定义  ----------*/
#define BackGround_NUM  99  //背景图存在图片目录的最后一个数组中
#endif
