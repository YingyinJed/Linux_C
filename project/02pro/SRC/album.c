#include "album.h"
/*
    函数名  ：Album_Start
    作  用  ：在管道内写入相应的命令
    输入参数：
    返 回 值：
*/
struct Album_user Album_Start(int PhotoNum,struct Lcd_Init LCD,struct Touch_val Touch,struct FileDir Dir_Photo)
{
    struct Album_user Album;
    if (Touch.x > 0 && Touch.x < 200)   //上一张   
    {
        Album.Touch_place = 2;//提示开发板用户有操作屏幕
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

    else if (Touch.x > 600 && Touch.x < 800)  //下一张 
    {
        Album.Touch_place = 2;//提示开发板用户有操作屏幕
        if (PhotoNum == (Dir_Photo.fileNum - 1))//因为从0开始数因此要-1
        { 
            PhotoNum = (Dir_Photo.fileNum - 1);
        }
        else
        {
            PhotoNum++;    
        }
        printf("Last Photo\n");
        printf("now the PhotoNum is %d\n",PhotoNum);
        printf("now the Photo's url is :%s\n",Dir_Photo.PhotoPath[PhotoNum]);
        open_bmp_left(LCD,Dir_Photo.PhotoPath[PhotoNum]);//显示当前图片
    }

    else if (Touch.x > 200 && Touch.x < 600)  //退出
    {
        printf("Quit\n");
        Album.Touch_place = 0;
        open_bmp_up(LCD,Dir_Photo.PhotoPath[BackGround_NUM]); 
    }
    else    //都没触碰
    {
        Album.Touch_place = 1;
    }
    
    Album.PhotoNum = PhotoNum;
    return Album;
}
