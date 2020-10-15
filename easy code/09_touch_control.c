#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>         //为了使用bzero(void *s,int n)
#include <linux/input.h>    //引入linux关于触摸屏的头文件
#include <dirent.h>         //引用目录类函数

#define TOUCH_PATH  "/dev/input/event0"
#define    LCD_PATH         "/dev/fb0"
#define    W                800
#define    H                480
#define    LCD_SIZE         W*H*4                
#define    BMP_SIZE         W*H*3 

char photo1[] = "./1.bmp";
char photo2[] = "./2.bmp";
char photo3[] = "./3.bmp";
char photo4[] = "./4.bmp";
char photo5[] = "./5.bmp";
char photo6[] = "./6.bmp";
char* photopath[] = {photo1,photo2,photo3,photo4,photo5,photo6};

struct input_event buf; //定义一个结构体获得x，y的坐标
struct dirent * pEnt = NULL;     //定义一个结构体获得目录数据


int open_bmp(char * PhotoPath)
{
    // 1.  打开LCD设备文件
    int fd_lcd = open( LCD_PATH , O_RDWR);
    if (-1 == fd_lcd)
    {
        perror("open LCD error");
        return -1 ;
    }
    // 内存映射,用于为LCD绘制图片
    int *p_lcd = (int *)mmap(NULL,	//需要内存映射的起始地址(通常设为 NULL, 代表让系统自动选定地址)
					LCD_SIZE,  //需要多大的一片内存(一字节为单位) LCD显示屏800*480个像素点 每个像素点占4字节
					PROT_READ | PROT_WRITE,  //映射区域可被读取 映射区域可被写入
					MAP_SHARED,					//对应射区域的写入数据会复制回文件内
					fd_lcd, 		//需要内存映射的文件的文件描述符
					0);			//参数offset为文件映射的偏移量, 通常设置为0
    if ( MAP_FAILED == p_lcd )
    {
        perror("mmap error");
        return -1 ;
    }
    // 2.  打开图片
    int fd_bmp = open( PhotoPath , O_RDONLY);
    lseek(fd_bmp, 54, SEEK_SET);    //便宜54个字节跳过头信息
    if (-1 == fd_bmp)
    {
        perror("open BMP error");
        return -1 ;
    }
    // 3.  读取图像中的BGR颜色数据
    char  buf_bmp [ BMP_SIZE ] ;
    int ret_val = read(fd_bmp, buf_bmp , BMP_SIZE );


    // 4.  把图像中的BGR 数据转换为 ARGB  24位转为 32位
    int  buf_lcd [ H ][ W ] ; 
    int x, y ;
    for ( y = 0; y < H ; y++)
    {
        for ( x = 0; x < W ; x++)
        {
            buf_lcd[y][x] = buf_bmp[(x+y*800)*3 + 0] << 0 | 
                            buf_bmp[(x+y*800)*3 + 1] << 8 | 
                            buf_bmp[(x+y*800)*3 + 2] << 16 ;
        }
    }
    // 5.  把转换后的数据显示到LCD文件中//H=480 W=800
    for ( y = 0; y < H; y++)
    {
        for ( x = 0; x < W; x++)
        { // 解引用
           *(p_lcd+x+y*800) = buf_lcd[479-y][x] ;
        }
    }

    // 6.  关闭文件释放资源
    close(fd_bmp);
    close(fd_lcd);
    munmap(p_lcd , LCD_SIZE);
    return 0;
}


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
    int x,y,num = 0;
    while (1)
    {
        while(1)
        {
            bzero(&buf,sizeof(buf));//置字节字符串s的前n个字符为0包括‘\0’

            read(ts_fd,&buf,sizeof(buf));
            if (buf.type == EV_ABS && buf.code == ABS_X)//获取X轴坐标   
            {
                x = buf.value;
                printf("(%d",x);//
            }
            if (buf.type == EV_ABS && buf.code == ABS_Y)//获取X轴坐标   
            {
                y = buf.value;
                printf(",%d)\n",y);//(x,y)
                printf("now num = %d\n",num);//打印现在要显示的图片
            }
            if (buf.value == 0)//松手检测
            {
                break;
            }
            if(x > 0 && x < 300)
            {
                num--;
                if (num < 0)
                {
                    num = 0;
                }
                x = y = 0;
            }
            if(x > 500 && x < 800)
            {
                num++;
                if (num > 6)
                {
                    num = 5;
                }
                x = y = 0;
            }
             if(x > 300 && x < 500)
             {
                 x = y = 0;
                 printf("quit\n");
                 break;
             }
            
            open_bmp(photopath[num]);
        }
    }

    close(ts_fd);
    
    return 0;
}

