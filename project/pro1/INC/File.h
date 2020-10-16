#ifndef _FILE_H_
#define _FILE_H_

/*----------引入自定义头文件----------*/
#include "main.h"

/*-------------------定义结构体---------------*/
struct FileDir
{
    int filename;
    char* PhotoPath[100];
};

/*-------------------定义函数---------------*/
struct FileDir dir_read(struct FileDir Dir);

#endif