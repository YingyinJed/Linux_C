#ifndef _music_mp3_H_
#define _music_mp3_H_

#include "main.h"

#define MPLAYER_TIPS1   "mplayer -quiet -slave -input file="
#define FIFOPATH_MP3    "/tmp/MP3"
#define MPLAYER_INIT_V  " -af volume=-30 "

//引用的结构体
struct Lcd_Init;
struct Touch_val;
struct FileDir;

/*-------------------定义结构体---------------*/
struct MP3_START
{
    int Touch_place;//记录触屏位置
    bool Flag_music;//判断音乐播放情况，该值需要返回
    int Music_Num;//记录现在播到哪一首
};

void MP3_Init(int fd_fifo,char * MP3path);
void MP3_Q_P(int fd_fifo,char * MP3path);
struct MP3_START MP3_Start( int fd_fifo_mp3,    //mp3管道的文件编号
                struct Lcd_Init LCD,//LCD显示
                struct FileDir Dir_Photo,//图片显示路径
                struct Touch_val Touch,//触摸屏判断
                struct FileDir Dir_MP3,//MP3文件路径
                bool Flag_music,//判断音乐播放情况，该值需要返回
                int Music_Num,//记录现在播到哪一首
                char * command//用于存放命令的数组
                );
#endif
