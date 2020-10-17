#include "main.h"

/*-------------------线程执行函数-------------------*/
void Touch_TS(struct Touch_val *Touch)
{
    while (1)
    {
        (*Touch) = TouchScan((*Touch));//扫描并获取现在的触摸位置
        printf("(%d,%d)\n",Touch->x,Touch->y);
    }
    
}

int main(int argc, char const *argv[])
{
    struct Lcd_Init LCD;        //定义LCD相关的结构体
    struct Touch_val Touch;     //定义触摸屏相关结构体
    struct FileDir Dir_Photo;   //定义获得图片目录

    char url_Photo[]= "/mnt/udisk/ZGT_YYJ/";//图片路径

    Dir_Photo.fileNum = 0;//初始化该结构体整型

    LCD = LCDInit();        //初始化LCD
    Touch = TouchInit();    //初始化触摸屏
    
    int PhotoNum = 0;

/*----------------   线程相关定义    -----------*/ 
    pthread_t Touch_pid;//定义一个用于扫描的线程


    pthread_create(&Touch_pid,NULL,Touch_TS,&Touch);
/*----------------   线程相关定义    -----------*/

/*----------------打开图片路径并读取图片路径下的普通文件-----------*/   
    for (int i = 0; i < 100; i++)
    {
        Dir_Photo.PhotoPath[i] = malloc(32);  //设置存储100个文件名的内存空间大小
    }
    
    Dir_Photo = dir_read_BMP(Dir_Photo,url_Photo);    //完成了在U盘中bmp_1目录下的文件读取
 /*----------------打开图片路径并读取图片路径下的普通文件------------------*/
    while (1)
    {
        //显示主界面
        open_bmp(LCD,Dir_Photo.PhotoPath[BackGround_NUM]); 
        Touch = TouchScan(Touch);//扫描并获取现在的触摸位置

        /*-----------------显示BMP文件-------------------*/
        if (Touch.x > 0 && Touch.x < 200 && Touch.y > 240)
        {
            Touch.x = Touch.y =0;   //读取完后及时清除坐标
            //真正进入到相册进程
            while (1)
            {
                Touch = TouchScan(Touch);//扫描并获取现在的触摸位置
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
                    if (PhotoNum == (Dir_Photo.fileNum - 2))//因为从0开始数且有一张背景图因此要-2
                    {
                        PhotoNum = (Dir_Photo.fileNum - 2);
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

        if (Touch.x > 200 && Touch.x < 400&&Touch.y > 240)   
        {
            
        }

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
