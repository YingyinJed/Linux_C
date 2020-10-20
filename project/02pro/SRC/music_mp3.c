#include "music_mp3.h"
//参数管道文件参数，MP3文件路径，控制命令
void MP3_Init(int fd_fifo,char * MP3path)
{
    //声明一个存储命令字符串的数组
    char command[130];
    //刚进入程序我们就将音乐播放器打开
    sprintf(command,"%s%s %s%s &",MPLAYER_TIPS1,FIFOPATH_MP3,MP3path,MPLAYER_INIT_V);
    printf("%s\n",command);
    //让系统先运行好播放器准备着
    system(command);
    //执行完命令后即使清除命令缓存区的内容
    sprintf(command,"");
    //然后暂停,等待操作
    sprintf(command,"pause\n");
    sleep(1); //由于播放需要时间，故延时1秒等文件打开
    Mk_Fifo(fd_fifo,command);
    //执行完命令后即使清除命令缓存区的内容
    sprintf(command,"");
}

void MP3_Q_P(int fd_fifo,char * MP3path)
{
    char command[100];
    sleep(1); //由于播放需要时间，故延时1秒等文件打开
    //先执行退出程序退出准备中的曲目
    sprintf(command,"quit\n");
    Mk_Fifo(fd_fifo,command);
    //执行完命令后即使清除命令缓存区的内容
    sprintf(command,"");  

    //再播放改变数量后的曲目
    sleep(1);//由于退出命令需要执行时间故延时1秒后执行
    sprintf(command,"%s%s %s%s &",MPLAYER_TIPS1,FIFOPATH_MP3,MP3path,MPLAYER_INIT_V);
    system(command);
    //执行完命令后即使清除命令缓存区的内容
    sprintf(command,""); 
}
