#include "Touch.h"

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

struct Touch_val TouchScan(struct Touch_val Touch)
{

    struct input_event buf;
    int count = 0;     //用来记录循环的次数
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
        if (ON_OFF_LET_GO == 1)
        {
            if (buf.value == 0)//松手检测
            { break; }
        }else
        {
            if (count < 2)//循环两次即可获得X、Y轴的值
            {
                count = 0;
                break;
            }
            
        }
        
    }
    return Touch;
}

void Touch_exit(struct Touch_val Touch)
{
    close(Touch.ts_fd);
}
