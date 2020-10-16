/*
    struct input_event
    {
        struct  timeval time;    //时间戳，精确到微秒
        _u16    type;           //输入事件类型
        _u16    code;           //具体时间描述
        _u16    value;          //具体动作描述
    }
    struct timeval
    {
        _time_t     tv_sec; //秒
        long int tv_usec;   //微秒
    }
    EV_SYN:事件分割标志
    EV_ABS:触摸屏坐标值ABS_X,ABS_Y,ABS_PRESSURE
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>         //为了使用bzero(void *s,int n)
#include <linux/input.h>    //引入linux关于触摸屏的头文件

#define TOUCH_PATH  "/dev/input/event0"

struct input_event buf;

int main(int argc, char const *argv[])
{
    //打开触摸屏
    int ts_fd = open(TOUCH_PATH,O_RDWR);
    if (ts_fd == -1)
    {
        perror("open ts failed");
        return -1;
    }

    //读取触摸屏数据
    int x,y;
    
        while(1)
        {
            bzero(&buf,sizeof(buf));//置字节字符串s的前n个字符为0包括‘\0’

            read(ts_fd,&buf,sizeof(buf));
            if (buf.type == EV_ABS && buf.code == ABS_X)//获取X轴坐标   
            {
                x = buf.value;
                printf("(%d,",x);//
            }
            if (buf.type == EV_ABS && buf.code == ABS_Y)//获取X轴坐标   
            {
                y = buf.value;
                printf("(%d,",y);//(x,y)
            }
            if (buf.value == 0)//松手检测
            {
                //break;
            }
            if(x > 0 && x < 300)
            {
                x = y = 0;
                printf("last photo\n");
            }
            if(x > 500 && x < 800)
            {
                x = y = 0;
                printf("next photo\n");
            }
            if(x > 300 && x < 500)
            {
                x = y = 0;
                printf("quit\n");
                break;
            }
        }
    

    close(ts_fd);
    return 0;
    
    
    return 0;
}

