#include "main.h"

int main(int argc, char const *argv[])
{
    struct Lcd_Init LCD;    //定义LCD相关的结构体
    struct Touch_val Touch; //定义触摸屏相关结构体
    struct FileDir Dir;

    Dir.filename = 0;//初始化该结构体整型

    LCD = LCDInit();        //初始化LCD
    Touch = TouchInit();    //初始化触摸屏
    
    int filenum = 0;
/*----------------打开图片路径并读取图片路径下的普通文件-----------*/   
    for (int i = 0; i < 100; i++)
    {
        Dir.PhotoPath[i] = malloc(32);           //设置存储100个文件名的内存空间大小
    }
    
    Dir = dir_read(Dir);
 /*----------------打开图片路径并读取图片路径下的普通文件------------------*/
    while (1)
    {
        Touch = TouchScan(Touch);
        if (Touch.x > 0 && Touch.x < 300)   
        {
            Touch.x = Touch.y = 0;//能退出扫描函数肯定是松手了因此将坐标归零,不然就重复判断了
            filenum++;
            if (filenum > Dir.filename)
            {
                filenum = Dir.filename;
            }
        }

        if (Touch.x > 300 && Touch.x < 500)   
        {
            printf("Quit\n");
            break;
        }

        if (Touch.x > 500 && Touch.x < 800)   
        {
            Touch.x = Touch.y = 0;//能退出扫描函数肯定是松手了因此将坐标归零
            filenum--;
            if (filenum < 0)
            {
                filenum = 0;
            }
        }
        open_bmp(LCD,Dir.PhotoPath[filenum]);
    }
    

    Touch_exit(Touch);//关闭触摸屏相关文件
    lcd_exit(LCD);  //关闭LCD
    return 0;
}
