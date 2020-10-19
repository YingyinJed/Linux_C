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
struct FileDir dir_read(struct FileDir Dir,char * url,char * type);//打开特定路径并返回路径信息到结构体

/*----------    宏定义  ----------*/
#define BackGround_NUM      99  //背景图存在图片目录的最后一个数组中

#define Music_Play          98  //音乐播放图放在目录的倒数第二个
#define Music_Stop          97  //音乐暂停图放在目录的倒数第三个

#define Photo_tip           96  //图片操作提示图

#define Gua_Tip             95  //刮刮乐提示语
#define Guaground           94  //刮刮乐背景图

#endif
