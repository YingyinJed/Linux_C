#ifndef _MAIN_H_
#define _MAIN_H_
/*----------引入系统相关头文件----------*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>         //引入配置内存，生成随机数相关头文件
#include <time.h>           //引入随机数相关头文件
#include <string.h>         //为了使用bzero(void *s,int n)
#include <linux/input.h>    //引入linux关于触摸屏的头文件
#include <dirent.h>         //引用目录类函数
#include <pthread.h>        //引用线程相关函数
#include <stdlib.h>         //引用系统进程相关头文件用于启动Madplay
#include <stdbool.h>        //由于用到了布尔型的变量因此要引入该头文件

/*----------引入自定义头文件----------*/
#include "ILI93xx.h"    //引入LCD的设置
#include "Touch.h"      //引入触摸屏相关设置
#include "File.h"       //引入读取文件头文件
#include "Fifo.h"       //引入管道相关头文件

#include "album.h"      //引用相册控制相关函数
#include "music_mp3.h"  //引用MP3控制相关函数
/*----------    宏定义  ----------*/

#endif
