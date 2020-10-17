#ifndef _TOUCH_H_
#define _TOUCH_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>         //引入配置内存相关头文件
#include <linux/input.h>    //引入linux关于触摸屏的头文件
#include <dirent.h>         //引用目录类函数
#include <linux/input.h>    //引入linux关于触摸屏的头文件
#include <string.h>         //为了使用bzero(void *s,int n)

#include "main.h"
#include "ILI93xx.h"
/*-------------------宏定义-------------------*/
#define    TOUCH_PATH  "/dev/input/event0"
#define    NOT_LET_GO    1   //有送手检测
#define    LET_GO_MODE   0   //没有松手检测模式
/*-------------------定义结构体---------------*/
struct Touch_val
{
    int ts_fd;//触摸屏的文件编号
    int x;  //读取触摸屏X轴
    int y;  //读取触摸屏Y轴
};

/*-------------------定义函数-----------------*/
struct Touch_val TouchInit(void);
struct Touch_val TouchScan(struct Touch_val Touch,int Touch_mode);
void Touch_exit(struct Touch_val Touch);

#endif
