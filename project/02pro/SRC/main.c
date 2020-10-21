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
            struct Album_user Album;
            //真正进入到相册进程
            while(1)
            {
                Album = Album_Start(PhotoNum,LCD,Touch,Dir_Photo);//退出时记录现在播到哪张图
                PhotoNum = Album.PhotoNum;
                if (Album.Touch_place == 2)//说明有触碰触摸屏
                {
                    Touch.x = Touch.y = 0;//扫描完成及时清除标志
                    Album.Touch_place = 1;//还原标志位
                }
                if (Album.Touch_place == 0 )//说明触摸位置时清0
                {
                    Touch.x = Touch.y = 0;//扫描完成及时清除标志
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

            struct MP3_START MP3;//定义用于返回的结构体
            //初始化
            MP3_Init(fd_fifo_mp3,Dir_MP3.PhotoPath[Music_Num]);
            while (1)
            {
                MP3 = MP3_Start(fd_fifo_mp3,LCD,Dir_Photo,Touch,Dir_MP3,Flag_music,Music_Num,command);
                Music_Num = MP3.Music_Num;
                Flag_music = MP3.Flag_music;
                if (MP3.Touch_place == 2)//如果操作了触摸屏及时清空坐标值
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                }
                else if (MP3.Touch_place == 1)//点击了退出则跳出循环
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
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

        /*-----------------视频播放-------------------*/
        if (Touch.x > 600 && Touch.x < 800 &&Touch.y > 240)   
        {
            
        }
        /*-----------------视频播放-------------------*/
    }  
    Touch_exit(Touch);//关闭触摸屏相关文件
    lcd_exit(LCD);  //关闭LCD
    close(fd_fifo_mp3);// 4. 关闭文件
    return 0;
}
