#ifndef _FILE_H_
#define _FILE_H_

/*----------引入自定义头文件----------*/
#include "main.h"

/*-------------------定义结构体---------------*/
struct FileDir
{
    int fileNum;//记录文件数量
    char* PhotoPath[100];//背景图存在最后一个路径中
};

/*-------------------定义函数---------------*/
struct FileDir dir_read(struct FileDir Dir,char * url);

#endif