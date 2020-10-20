#include "main.h"

/*-------------------定义全局变量-------------------*/
int Touch_mode = NOT_LET_GO;//定义一个参数用于更改触摸屏扫描模式，默认为含松手检测的扫描模式
int PhotoNum = 0;   //记录现在放映的是第几张图片
int Music_Num = 0;//记录现在播放的是第几首歌
/*-------------------定义全局变量-------------------*/

/*-------------------线程执行函数-------------------*/
void* Touch_TS(void* Touch)
{
    struct Touch_val *Touch_POI;
    Touch_POI = Touch;
    while (1)
    {
        (*Touch_POI) = TouchScan((*Touch_POI),Touch_mode);//扫描并获取现在的触摸位置
    }
}
//void* MPLAYER_PLAY()
/*-------------------线程执行函数-------------------*/

/*-------------------主运行函数-------------------*/
int main(int argc, char const *argv[])
{
    struct Lcd_Init LCD;        //定义LCD相关的结构体

    struct Touch_val Touch;     //定义触摸屏相关结构体
    struct Touch_val *Touch_POI;//定义结构体指针用于线程赋值
    Touch_POI = &Touch;         //该指针指向后面引用的结构体

    struct FileDir Dir_Photo;   //定义获得图片目录
    struct FileDir Dir_MP3;     //定义获得MP3目录

    char url_Photo[]= "/ZGT/Picture/";  //图片路径
    char url_MP3[] = "/ZGT/MP3/";       //MP3路径

    Dir_Photo.fileNum = 0;//初始化该结构体整型
    Dir_MP3.fileNum = 0;

    LCD = LCDInit();        //初始化LCD
    Touch = TouchInit();    //初始化触摸屏
    
    int fd_fifo_mp3 = Fifo_Init(FIFOPATH_MP3);//初始化MP3管道

/*----------------   线程相关定义    -----------*/ 
    pthread_t Touch_pid;//定义一个用于扫描触摸屏的线程
    pthread_t MPLAYER;  //定义一个线程用于MPLAYER播放使用
    //创建并配置线程相关参数
    pthread_create(&Touch_pid,NULL,Touch_TS,(void *)Touch_POI);//配置扫描触摸屏线程
    //pthread_create(&MPLAYER,NULL,MPLAYER_PLAY,(void *)Touch_POI);
/*----------------   线程相关定义    -----------*/

/*----------------打开图片路径并读取图片路径下的普通文件-----------*/   
    for (int i = 0; i < 100; i++)
    {
        //设置存储100个文件名的内存空间大小
        Dir_Photo.PhotoPath[i] = malloc(300);//存储图片文件名，路径的长度为300字符以内
        Dir_MP3.PhotoPath[i] = malloc(300);  //存储MP3文件名，路径的长度为300字符以内
    }
    
    Dir_Photo = dir_read(Dir_Photo,url_Photo,type_bmp);    //读取图片目录下的文件
    Dir_MP3 = dir_read(Dir_MP3,url_MP3,type_mp3);      //读取MP3目录下的文件
 /*----------------打开图片路径并读取图片路径下的普通文件------------------*/
    //显示主界面
    open_bmp(LCD,Dir_Photo.PhotoPath[BackGround_NUM]); 
    while (1)
    {
        Touch_mode = NOT_LET_GO;//每一次回到著见面都返回到有松手检测的模式
        /*-----------------显示BMP文件-------------------*/
        if (Touch.x > 0 && Touch.x < 200 && Touch.y > 240)
        {
            Touch.x = Touch.y =0;   //读取完后及时清除坐标
            open_bmp(LCD,Dir_Photo.PhotoPath[Photo_tip]);//打开图片操作提示
            while (1)
            {
                if(Touch.x != 0 && Touch.y != 0)//有触碰触摸屏程序才继续
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                    open_bmp_down(LCD,Dir_Photo.PhotoPath[PhotoNum]);//显示当前图片
                    break;
                }
            }
            //真正进入到相册进程
            while (1)
            {
                if (Touch.x > 0 && Touch.x < 200)   //上一张   
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                    if (PhotoNum == 0)
                    {
                        PhotoNum = 0;
                    }else
                    {
                        PhotoNum--;
                    }
                    printf("Last Photo\n");
                    printf("now the PhotoNum is %d\n",PhotoNum);
                    printf("now the Photo's url is :%s\n",Dir_Photo.PhotoPath[PhotoNum]);
                    open_bmp_right(LCD,Dir_Photo.PhotoPath[PhotoNum]);//显示当前图片
                }
                if (Touch.x > 600 && Touch.x < 800)  //下一张 
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                    if (PhotoNum == (Dir_Photo.fileNum - 1))//因为从0开始数因此要-1
                    {   PhotoNum = (Dir_Photo.fileNum - 1);}
                    else
                    {   PhotoNum++; }
                    open_bmp_left(LCD,Dir_Photo.PhotoPath[PhotoNum]);//显示当前图片
                }
                if (Touch.x > 200 && Touch.x < 600)  //退出
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                    printf("Quit\n");
                    open_bmp_up(LCD,Dir_Photo.PhotoPath[BackGround_NUM]); 
                    break;
                }
            }
        }
        /*-----------------显示BMP文件-------------------*/
        
        /*-----------------播放音乐文件-------------------*/
        if (Touch.x > 200 && Touch.x < 400&&Touch.y > 240)   
        {
            Touch.x = Touch.y =0;   //读取完后及时清除坐标
            //显示音乐界面
            open_bmp_down(LCD,Dir_Photo.PhotoPath[Music_Stop]);
            //声明一个布尔类型记录音乐播放状态
            bool Flag_music = false;
            
            char command[200];
            //初始化
            MP3_Init(fd_fifo_mp3,Dir_MP3.PhotoPath[Music_Num]);
            while (1)
            {
                //点击上一曲
                if (Touch.x > 160 && Touch.x < 260 && Touch.y > 95 && Touch.y < 286)
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
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
                if (Touch.x > 300 && Touch.x < 465 && Touch.y > 95 && Touch.y < 286)
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                    sleep(1);//延时1秒防止操作不了
                    //执行播放/继续命令
                    sprintf(command,"pause\n");
                    Mk_Fifo(fd_fifo_mp3,command);
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
                if (Touch.x > 540 && Touch.x < 620 && Touch.y > 95 && Touch.y < 286)
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
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
                if (Touch.x > 690 && Touch.x < 800 && Touch.y > 95 && Touch.y < 286)
                {
                    //改变播放状态
                    Flag_music = true;
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                    open_bmp(LCD,Dir_Photo.PhotoPath[Music_Play]);//运行播放音乐界面
                    //执行快进10秒
                    sprintf(command,"seek 10\n");
                    Mk_Fifo(fd_fifo_mp3,command);
                    //执行完命令后即使清除命令缓存区的内容
                    sprintf(command,"");
                }
                //进度后退10s
                if (Touch.x > 0 && Touch.x < 110 && Touch.y > 95 && Touch.y < 286)
                {
                    //改变播放状态
                    Flag_music = true;
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                    open_bmp(LCD,Dir_Photo.PhotoPath[Music_Play]);//运行播放音乐界面
                    sleep(1);//延时1秒防止操作不了
                    //执行后退10秒
                    sprintf(command,"seek -10\n");
                    Mk_Fifo(fd_fifo_mp3,command);
                    //执行完命令后即使清除命令缓存区的内容
                    sprintf(command,"");
                }
                //音量+10
                if (Touch.x > 60 && Touch.x < 165 && Touch.y > 330 && Touch.y < 450)
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                    //执行音量+10
                    sprintf(command,"volume -10\n");
                    Mk_Fifo(fd_fifo_mp3,command);
                    //执行完命令后即使清除命令缓存区的内容
                    sprintf(command,"");
                    
                }

                //音量-10
                if (Touch.x > 620 && Touch.x < 740 && Touch.y > 330 && Touch.y < 450)
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                    //执行音量-10
                    sprintf(command,"volume 10\n");
                    Mk_Fifo(fd_fifo_mp3,command);
                    //执行完命令后即使清除命令缓存区的内容
                    sprintf(command,"");
                }

                //点击返回菜单
                if (Touch.x > 250 && Touch.x < 540 && Touch.y > 335 && Touch.y < 435)
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                    //执行退出程序退出准备中的曲目
                    sprintf(command,"quit\n");
                    Mk_Fifo(fd_fifo_mp3,command);
                    //执行完命令后即使清除命令缓存区的内容
                    sprintf(command,""); 
                    printf("Quit\n");
                    //显示主界面
                    open_bmp_up(LCD,Dir_Photo.PhotoPath[BackGround_NUM]); 
                    break;
                }
                //根据坐标点判断：播放音乐/暂停音乐/上一曲/下一曲/返回菜单
            }
            
        }
        /*-----------------播放音乐文件-------------------*/

        /*-----------------刮刮乐-------------------*/
        if (Touch.x > 400 && Touch.x < 600&&Touch.y > 240)   
        {
            Touch.x = Touch.y =0;   //读取完后及时清除坐标
            open_bmp_up(LCD,Dir_Photo.PhotoPath[Gua_Tip]);//先显示刮刮乐提示图
            while (1)
            {
                if(Touch.x != 0 && Touch.y != 0)//有触碰触摸屏程序才继续
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                    open_bmp(LCD,Dir_Photo.PhotoPath[Guaground]);//显示刮刮乐背景图
                    break;
                }
            }
            Touch_mode = LET_GO_MODE;//关闭松手检测
            int rand_num;//定义一个随机数用于使刮刮乐刮开的图片每次不同
            srand((int)time(0));//设置随机数种子
            rand_num =  (int) (((rand()%(Dir_Photo.fileNum))+0));
            while (1)
            {
                if (Touch.x != 0 && Touch.y != 0)//防止一开始就输入了(0,0)这个值
                {
                    open_bmp_X_Y(LCD,Dir_Photo.PhotoPath[rand_num],Touch.x,Touch.y);//图片在触摸屏触摸到的位置上显示一部分
                }
                if (Touch.x > 650 && Touch.x < 800 && Touch.y > 0 && Touch.y < 40)
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                    open_bmp_down(LCD,Dir_Photo.PhotoPath[BackGround_NUM]);//显示主界面背景图
                    break;
                }
            }
        }
        /*-----------------刮刮乐-------------------*/

        if (Touch.x > 600 && Touch.x < 800 &&Touch.y > 240)   
        {
            
        }
    }  
    Touch_exit(Touch);//关闭触摸屏相关文件
    lcd_exit(LCD);  //关闭LCD
    close(fd_fifo_mp3);// 4. 关闭文件
    return 0;
}
