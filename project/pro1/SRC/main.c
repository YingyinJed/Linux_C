#include "main.h"



int main(int argc, char const *argv[])
{
    struct Lcd_Init LCD;    //定义LCD相关的结构体
    struct Touch_val Touch; //定义触摸屏相关结构体

    LCD = LCDInit();        //初始化LCD
    Touch = TouchInit();    //初始化触摸屏
    char *PhotoPath[100];   //可以存储100个图片文件

    int filename;
    int filenum;
/*----------------打开图片路径并读取图片路径下的普通文件-----------*/   
    DIR *pDir = NULL;//创建一个DIR*存储打开的路径
    struct dirent * pEnt = NULL;//创建一个存储路径中读到的信息
    char str[64];        //用于存储当前图片的图片路径
    //打开图片目录
    pDir = opendir("./Picture/");
    if (NULL == pDir)
    {
        perror("opendir");
        return -1;
    }
    //读取目录中的内容
    while (1)
    {
            
        pEnt = readdir(pDir);
        if(pEnt != NULL)
            {
            if (pEnt->d_type == DT_REG)//获得目录内的普通文件
            {
                sprintf(str,"./Picture/%s",pEnt->d_name);
                PhotoPath[filename] = malloc(32);           //设置存储文件名的内存空间大小
                strcpy(PhotoPath[filename],str);
                filename++;
                usleep(300000);
            }
        }
        else
        {
            closedir(pDir);//读取完成关闭目录
            break;
        }
    }   
 /*----------------打开图片路径并读取图片路径下的普通文件------------------*/
    printf("Star Scan!\n");
    while (1)
    {
        Touch = TouchScan(Touch);
        if (Touch.x > 0 && Touch.x < 300)   
        {
            Touch.x = Touch.y = 0;//能退出扫描函数肯定是松手了因此将坐标归零,不然就重复判断了
            filenum++;
            if (filenum > filename)
            {
                filenum = filename;
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
        open_bmp(LCD,PhotoPath[filenum]);
    }
    

    Touch_exit(Touch);//关闭触摸屏相关文件
    lcd_exit(LCD);  //关闭LCD
    return 0;
}
