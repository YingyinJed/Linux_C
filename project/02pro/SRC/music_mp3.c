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
    WR_Fifo(fd_fifo,command);
    //执行完命令后即使清除命令缓存区的内容
    sprintf(command,"");
}

void MP3_Q_P(int fd_fifo,char * MP3path)
{
    char command[100];
    sleep(1); //由于播放需要时间，故延时1秒等文件打开
    //先执行退出程序退出准备中的曲目
    sprintf(command,"quit\n");
    WR_Fifo(fd_fifo,command);
    //执行完命令后即使清除命令缓存区的内容
    sprintf(command,"");  

    //再播放改变数量后的曲目
    sleep(1);//由于退出命令需要执行时间故延时1秒后执行
    sprintf(command,"%s%s %s%s &",MPLAYER_TIPS1,FIFOPATH_MP3,MP3path,MPLAYER_INIT_V);
    system(command);
    //执行完命令后即使清除命令缓存区的内容
    sprintf(command,""); 
}

struct MP3_START MP3_Start( int fd_fifo_mp3,    //mp3管道的文件编号
                struct Lcd_Init LCD,//LCD显示
                struct FileDir Dir_Photo,//图片显示路径
                struct Touch_val Touch,//触摸屏判断
                struct FileDir Dir_MP3,//MP3文件路径
                bool Flag_music,//判断音乐播放情况，该值需要返回
                int Music_Num,//记录现在播到哪一首
                char * command//用于存放命令的数组
                )
{
    struct MP3_START MP3;//定义用于返回的结构体
    //点击上一曲
    if (Touch.x > 160 && Touch.x < 260 && Touch.y > 95 && Touch.y < 286)
    {
        MP3.Touch_place = 2;//记录触摸位置属于哪种事件
        Flag_music = true;//无论原状态如何现在都是播放音乐的状态
        open_bmp(LCD,Dir_Photo.PhotoPath[Music_Play]);//运行播放音乐界面
        //改变播放的曲目
        if (Music_Num <= 0)
        { Music_Num = 0; }
        else
        { Music_Num--; }
        MP3_Q_P(fd_fifo_mp3,Dir_MP3.PhotoPath[Music_Num]);         
    }
    //点击播放/暂停键
    else if (Touch.x > 300 && Touch.x < 465 && Touch.y > 95 && Touch.y < 286)
    {
        MP3.Touch_place = 2;//记录按压位置属于哪种事件
        sleep(1);//延时1秒防止操作不了
        //执行播放/继续命令
        sprintf(command,"pause\n");
        WR_Fifo(fd_fifo_mp3,command);
        //执行完命令后即使清除命令缓存区的内容
        sprintf(command,""); 
        //判断音乐是否处于播放状态
        if(Flag_music == false)
        {
            //改变播放状态
            Flag_music = true;
            open_bmp(LCD,Dir_Photo.PhotoPath[Music_Play]);//运行播放音乐界面 
        }else
        {
            Flag_music = false;//改变音乐状态
            open_bmp(LCD,Dir_Photo.PhotoPath[Music_Stop]);//运行暂停播放页面
        }
    }
    //点击下一曲
    else if (Touch.x > 540 && Touch.x < 620 && Touch.y > 95 && Touch.y < 286)
    {
        MP3.Touch_place = 2;//记录按压位置属于哪种事件
        Flag_music = true;//无论原状态如何现在都是播放音乐的状态
        open_bmp(LCD,Dir_Photo.PhotoPath[Music_Play]);//运行播放音乐界面
        //改变播放的曲目
        if (Music_Num == (Dir_MP3.fileNum - 1))
        { Music_Num = (Dir_MP3.fileNum - 1); }
        else
        { Music_Num++; }
        MP3_Q_P(fd_fifo_mp3,Dir_MP3.PhotoPath[Music_Num]);  
    }
    //进度前进10s
    else if (Touch.x > 690 && Touch.x < 800 && Touch.y > 95 && Touch.y < 286)
    {
        //改变播放状态
        Flag_music = true;
        MP3.Touch_place = 2;//记录按压位置属于哪种事件
        open_bmp(LCD,Dir_Photo.PhotoPath[Music_Play]);//运行播放音乐界面
        //执行快进10秒
        sprintf(command,"seek 10\n");
        WR_Fifo(fd_fifo_mp3,command);
        //执行完命令后即使清除命令缓存区的内容
        sprintf(command,"");
    }
    //进度后退10s
    else if (Touch.x > 0 && Touch.x < 110 && Touch.y > 95 && Touch.y < 286)
    {
        //改变播放状态
        Flag_music = true;
        MP3.Touch_place = 2;//记录按压位置属于哪种事件
        open_bmp(LCD,Dir_Photo.PhotoPath[Music_Play]);//运行播放音乐界面
        sleep(1);//延时1秒防止操作不了
        //执行后退10秒
        sprintf(command,"seek -10\n");
        WR_Fifo(fd_fifo_mp3,command);
        //执行完命令后即使清除命令缓存区的内容
        sprintf(command,"");
    }
    //音量+10
    else if (Touch.x > 60 && Touch.x < 165 && Touch.y > 330 && Touch.y < 450)
    {
        MP3.Touch_place = 2;//记录按压位置属于哪种事件
        //执行音量+10
        sprintf(command,"volume -10\n");
        WR_Fifo(fd_fifo_mp3,command);
        //执行完命令后即使清除命令缓存区的内容
        sprintf(command,"");
        
    }

    //音量-10
    else if (Touch.x > 620 && Touch.x < 740 && Touch.y > 330 && Touch.y < 450)
    {
        MP3.Touch_place = 2;//记录按压位置属于哪种事件
        //执行音量-10
        sprintf(command,"volume 10\n");
        WR_Fifo(fd_fifo_mp3,command);
        //执行完命令后即使清除命令缓存区的内容
        sprintf(command,"");
    }

    //点击返回菜单
    else if (Touch.x > 250 && Touch.x < 540 && Touch.y > 335 && Touch.y < 435)
    {
        MP3.Touch_place = 1;//记录按压位置属于哪种事件
        //执行退出程序退出准备中的曲目
        sprintf(command,"quit\n");
        WR_Fifo(fd_fifo_mp3,command);
        //执行完命令后即使清除命令缓存区的内容
        sprintf(command,""); 
        printf("Quit\n");
        //显示主界面
        open_bmp_up(LCD,Dir_Photo.PhotoPath[BackGround_NUM]); 
    }
    //说明什么都没按
    else
    {
        MP3.Touch_place = 0;//记录按压位置属于哪种事件
    }
    MP3.Music_Num = Music_Num;

    MP3.Flag_music = Flag_music;

    return MP3;
}
