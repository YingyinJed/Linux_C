#include "Touch.h"

/*
    函数名  ：TouchInit
    作  用  ：初始化触摸屏
    输入参数：无
    返 回 值：返回结构体内含触摸屏文件的文件编号
*/
struct Touch_val TouchInit(void)
{
    struct Touch_val Touch;
    //打开触摸屏
    Touch.ts_fd = open(TOUCH_PATH,O_RDWR);
    if (Touch.ts_fd == -1)
    {
        perror("open ts failed");
        return Touch;
    }
    return Touch;
}
/*
    函数名  ：TouchScan
    作  用  ：扫描触摸屏，返回触摸的坐标，在被触摸前不会跳出
    输入参数：用于存储返回参数的结构体
             设置扫描模式，1为有松手检测，0为没有松手检测
    返 回 值：返回触摸屏触摸情况结构体内含X，Y坐标
*/
struct Touch_val TouchScan(struct Touch_val Touch,int Touch_mode)
{
    struct input_event buf;
    int count = 0;
    while (1)
    { 
        bzero(&buf,sizeof(buf));//置字节字符串s的前n个字符为0包括‘\0’

        read(Touch.ts_fd,&buf,sizeof(buf));//读取触摸屏内容

        if (buf.type == EV_ABS && buf.code == ABS_X)//获取X轴坐标   
        {
            Touch.x = buf.value;
        }
        if (buf.type == EV_ABS && buf.code == ABS_Y)//获取X轴坐标   
        {
            Touch.y = buf.value;
        }
        count++;
        if (Touch_mode == 1)//扫描模式有松手检测
        {
            if (buf.type == EV_KEY && buf.code == BTN_TOUCH &&buf.value == 0)//松手检测
            { break; }
        }else
        {
            if (count == 1)
            {
                break;
            }
            
        }  
    }
    return Touch;
}
/*
    函数名  ：Touch_exit
    作  用  ：关闭触摸屏相关文件
    输入参数：触摸屏结构体，内含需要关闭的文件编号
    返 回 值：无
*/
void Touch_exit(struct Touch_val Touch)
{
    close(Touch.ts_fd);
}
