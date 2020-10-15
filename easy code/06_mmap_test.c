#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define     LCD_PATH    "/dev/fb0"

int main(int argc, char const *argv[])
{
    //打开文件
    int fd_lcd = open(LCD_PATH,O_RDWR);
    
    if (fd_lcd == -1)
    {
        perror("open lcd error");
        return -1;
    }
    //内存映射
    int*p_lcd = mmap(
        NULL,                   //由系统分配
        800*480*4,              //映射区域大小，像素点数量*字节长度
        PROT_READ | PROT_WRITE, //可读可写
        MAP_SHARED,             //对内存的操作会被复制到文件中
        fd_lcd,                 //需要映射的文件描述符
        0                       //偏移量为0，文件内容一一对应
    );
    // 写入RGB
    int color_s = 0xFFFAFA,
        color_b = 0x000000,
        color_y = 0xFFFF00,
        color_g = 0x00FF00,
        color_r = 0XFF0000,
        change_x = 1,
        change_y = 1;
        
        while(1)
        {
            for (int x = 0; x < 800; x++)
            {
                for (int y = 0; y < 480; y++)
                { // 解引用
                    if((x < (2*(change_x+1)) && x > (2 * change_x) )|| (y < (2*(change_y+1)) && y > (2 * change_y)))
                    {
                        *(p_lcd+x+y*800) = color_b;
                    }
                    else
                    {
                        *(p_lcd+x+y*800) = color_y;
                    }
                }
            }
            change_x++,
            change_y++;
            if (change_x == 400)
            {
                change_x = 1;
            }
            else  if(change_y == 240 )
            {
                change_y = 1;
            }  
        }
    // 关闭文件
    close(fd_lcd);

    // 释放内存
    munmap(p_lcd , 800*480*4);
    return 0;
}
