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
    
    int color_orange = 0xFF4500;
    int color_green = 0x54FF9F;
    int color_seagreen = 0x90EE90;
    int color_purple = 0xA020F0;
    int color_durk_bule = 0x8A2BE2;
    int color_write = 0xF8F8FF;
    for (int y = 0; y < 480; y++)
    {
        for (int x = 0; x < 800; x++)
        {   // 解引用
            if( y > 96*2 && y < 96*3) 
            {
                if(x > 160*2 && x < 160*3)
                {*(p_lcd+x+y*800) = color_orange;}
                else
                {
                    {*(p_lcd+x+y*800) = color_write;}
                }
                
            }else
            {
                {*(p_lcd+x+y*800) = color_write;}
            }
              
        }
        
    }
    
    close(fd_lcd);
    // 释放内存
    munmap(p_lcd , 800*480*4);
    return 0;
}
