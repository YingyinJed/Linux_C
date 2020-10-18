#include "main.h"

/*-------------------线程执行函数-------------------*/
void* Touch_TS(void* Touch)
{
    struct Touch_val *Touch_POI;
    Touch_POI = Touch;
    while (1)
    {
        (*Touch_POI) = TouchScan((*Touch_POI),NOT_LET_GO);//扫描并获取现在的触摸位置，扫描模式有松手检测
    }
}
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
    
    int PhotoNum = 0;

/*----------------   线程相关定义    -----------*/ 
    pthread_t Touch_pid;//定义一个用于扫描的线程

    //创建并配置线程相关参数
    pthread_create(&Touch_pid,NULL,Touch_TS,(void *)Touch_POI);
/*----------------   线程相关定义    -----------*/

/*----------------打开图片路径并读取图片路径下的普通文件-----------*/   
    for (int i = 0; i < 100; i++)
    {
        //设置存储100个文件名的内存空间大小
        Dir_Photo.PhotoPath[i] = malloc(32);//存储图片文件名
        Dir_MP3.PhotoPath[i] = malloc(32);  //存储MP3文件名
    }
    
    Dir_Photo = dir_read(Dir_Photo,url_Photo);    //读取图片目录下的文件
    Dir_MP3 = dir_read(Dir_MP3,url_MP3);      //读取MP3目录下的文件
 /*----------------打开图片路径并读取图片路径下的普通文件------------------*/
    while (1)
    {
        //显示主界面
        open_bmp(LCD,Dir_Photo.PhotoPath[BackGround_NUM]); 
        //Touch = TouchScan(Touch);//扫描并获取现在的触摸位置

        /*-----------------显示BMP文件-------------------*/
        if (Touch.x > 0 && Touch.x < 200 && Touch.y > 240)
        {
            Touch.x = Touch.y =0;   //读取完后及时清除坐标
            //真正进入到相册进程
            while (1)
            {
                //Touch = TouchScan(Touch);//扫描并获取现在的触摸位置
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
                }
                if (Touch.x > 600 && Touch.x < 800)  //下一张 
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                    if (PhotoNum == (Dir_Photo.fileNum - 1))//因为从0开始数因此要-1
                    {
                        PhotoNum = (Dir_Photo.fileNum - 1);
                    }else
                    {
                        PhotoNum++;
                    }
                    printf("Next Photo\n");
                    printf("now the PhotoNum is %d\n",PhotoNum);
                    printf("now the Photo's url is :%s\n",Dir_Photo.PhotoPath[PhotoNum]);
                }
                if (Touch.x > 200 && Touch.x < 600)  //退出
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                    printf("Quit\n");
                    break;
                }
                open_bmp(LCD,Dir_Photo.PhotoPath[PhotoNum]);//显示当前图片
            }
        }
        /*-----------------显示BMP文件-------------------*/
        
        /*-----------------播放音乐文件-------------------*/
        if (Touch.x > 200 && Touch.x < 400&&Touch.y > 240)   
        {
            Touch.x = Touch.y =0;   //读取完后及时清除坐标
            //显示音乐界面
            open_bmp(LCD,Dir_Photo.PhotoPath[Music_Stop]);
            //声明一个布尔类型记录音乐播放状态
            bool Flag_music = false;
            //再声明一个变量记录第一次去按下按键的时候
            bool First_Touch_music = false;
            //声明3个添加命令的字符串
            char madplay[] = "madplay ";
            char volume [] = " -a -20 &";
            char command[45];
            //声明一个变量记录现在播放的是第几首歌
            int Music_Num = 0;
            while (1)
            {
                //点击上一曲
                if (Touch.x > 70 && Touch.x < 225 && Touch.y > 120 && Touch.y < 320)
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                    Flag_music = true;//无论原状态如何现在都是播放音乐的状态
                    open_bmp(LCD,Dir_Photo.PhotoPath[Music_Play]);//运行播放音乐界面
                    if (Music_Num <= 0)
                    {
                        Music_Num = 0;
                    }else
                    {
                        Music_Num--;
                    }

                    if (First_Touch_music == false)//如果该按钮是第一个被按下的
                    {
                        //播放音乐,添加取址符是让进程后台执行
                        //先获得命令字符串
                        strcat(command,madplay);
                        //再获得路径
                        strcat(command,Dir_MP3.PhotoPath[Music_Num]);
                        //再获得音量命令
                        strcat(command,volume);
                        //执行命令
                        printf("%s\n",command);
                        system(command);
                        //执行完命令后即使清除命令缓存区的内容
                        sprintf(command,"");
                    }else//如果不是第一次按下的说明已经有音乐进程
                    {
                        system("killall -KILL madplay");//先删除上一个播放进程
                        //播放音乐,添加取址符是让进程后台执行
                        //先获得命令字符串
                        strcat(command,madplay);
                        //再获得路径
                        strcat(command,Dir_MP3.PhotoPath[Music_Num]);
                        //再获得音量命令
                        strcat(command,volume);
                        //执行命令
                        printf("%s\n",command);
                        system(command);
                        //执行完命令后即使清除命令缓存区的内容
                        sprintf(command,"");
                    }   
                }
                //点击播放/暂停键
                if (Touch.x > 300 && Touch.x < 475 && Touch.y > 120 && Touch.y < 320)
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                    //判断音乐是否处于播放状态
                    if(Flag_music == false)
                    {
                        open_bmp(LCD,Dir_Photo.PhotoPath[Music_Play]);//运行播放音乐界面
                        if (First_Touch_music == false)//如果这是第一次被按下则开启播放
                        {
                            First_Touch_music = true;//清除第一次播放的标志位
                            Flag_music = true;//改变音乐状态
                            //播放音乐,添加取址符是让进程后台执行
                            //先获得命令字符串
                            strcat(command,madplay);
                            //再获得路径
                            strcat(command,Dir_MP3.PhotoPath[Music_Num]);
                            //再获得音量命令
                            strcat(command,volume);
                            //执行命令
                            printf("%s\n",command);
                            system(command);
                            //执行完命令后即使清除命令缓存区的内容
                            sprintf(command,"");
                        }else//如果不是第一次播放说明是暂停，只要继续就好了
                        {
                            Flag_music = true;//改变音乐状态
                            system("killall -CONT madplay");//继续播放音乐
                        }  
                    }else
                    {
                        Flag_music = false;//改变音乐状态
                        system("killall -STOP madplay");//暂停音乐
                        open_bmp(LCD,Dir_Photo.PhotoPath[Music_Stop]);//运行暂停播放页面
                    }
                }
                //点击下一曲
                if (Touch.x > 555 && Touch.x < 715 && Touch.y > 120 && Touch.y < 320)
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                    Flag_music = true;//无论原状态如何现在都是播放音乐的状态
                    open_bmp(LCD,Dir_Photo.PhotoPath[Music_Play]);//运行播放音乐界面
                    if (Music_Num == (Dir_MP3.fileNum - 1))
                    {
                        Music_Num = (Dir_MP3.fileNum - 1);
                    }else
                    {
                        Music_Num++;
                    }
                    if (First_Touch_music == false)//如果该按钮是第一个被按下的
                    {
                        //播放音乐,添加取址符是让进程后台执行
                        //先获得命令字符串
                        strcat(command,madplay);
                        //再获得路径
                        strcat(command,Dir_MP3.PhotoPath[Music_Num]);
                        //再获得音量命令
                        strcat(command,volume);
                        //执行命令
                        printf("%s\n",command);
                        system(command);
                        //执行完命令后即使清除命令缓存区的内容
                        sprintf(command,"");
                    }else//如果不是第一次按下的说明已经有音乐进程
                    {
                        system("killall -KILL madplay");//先删除上一个播放进程
                        //播放音乐,添加取址符是让进程后台执行
                        //先获得命令字符串
                        strcat(command,madplay);
                        //再获得路径
                        strcat(command,Dir_MP3.PhotoPath[Music_Num]);
                        //再获得音量命令
                        strcat(command,volume);
                        //执行命令
                        printf("%s\n",command);
                        system(command);
                        //执行完命令后即使清除命令缓存区的内容
                        sprintf(command,"");
                    }
                }
                //点击返回菜单
                if (Touch.x > 250 && Touch.x < 540 && Touch.y > 335 && Touch.y < 435)
                {
                    Touch.x = Touch.y =0;   //读取完后及时清除坐标
                    system("killall -KILL madplay");//退出播放进程
                    printf("Quit\n");
                    break;
                }
                //根据坐标点判断：播放音乐/暂停音乐/上一曲/下一曲/返回菜单
            }
            
        }
        /*-----------------播放音乐文件-------------------*/

        if (Touch.x > 400 && Touch.x < 600&&Touch.y > 240)   
        {
            
        }
        if (Touch.x > 600 && Touch.x < 800 &&Touch.y > 240)   
        {
            
        }
    }  
    Touch_exit(Touch);//关闭触摸屏相关文件
    lcd_exit(LCD);  //关闭LCD
    return 0;
}
