#ifndef _ALBUM_H_
#define _ALBUM_H_

#include "main.h"

//引用的结构体
struct Lcd_Init;
struct Touch_val;
struct FileDir;

//自定义结构体
struct Album_user
{
    int PhotoNum;
    int Touch_place;
};


struct Album_user Album_Start(int PhotoNum,struct Lcd_Init LCD,struct Touch_val Touch,struct FileDir Dir_Photo);

#endif