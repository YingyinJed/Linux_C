#include "main.h"

int main(int argc, char const *argv[])
{
    struct Lcd_Init LCD;        //定义LCD相关的结构体
    struct Touch_val Touch;     //定义触摸屏相关结构体
    struct FileDir Dir_Photo;   //定义获得图片目录

    char url_Photo[]= "/mnt/udisk/ZGT_YYJ/";//图片路径

    Dir_Photo.filename = 0;//初始化该结构体整型

    LCD = LCDInit();        //初始化LCD
    Touch = TouchInit();    //初始化触摸屏
    
    int filenum = 0;
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
        while (1)
        {
            Touch = TouchScan(Touch);//扫描并获取现在的触摸位置
            if (Touch.x > 0 && Touch.x < 300)   
            {
                filenum++;
                if (filenum > Dir_Photo.filename)
                {
                    filenum = Dir_Photo.filename;
                }
            }

            if (Touch.x > 300 && Touch.x < 500)   
            {
                printf("Quit\n");
                break;
            }

            if (Touch.x > 500 && Touch.x < 800)   
            {
                filenum--;
                if (filenum < 0)
                {
                    filenum = 0;
                }
            }
        }  
    }
    

    Touch_exit(Touch);//关闭触摸屏相关文件
    lcd_exit(LCD);  //关闭LCD
    return 0;
}
