#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define     FIFO_PATH       "/tmp/ZhaoErGou"

int main(int argc, char const *argv[])
{
    // 0. 检查管道文件是否存在
    if (access( FIFO_PATH , F_OK))
    {
         // 1. 创建管道文件   mkfifo( );
        if(mkfifo(FIFO_PATH , 0666 ))
        {
            perror("mkfifo  error ");
        } 
    }
    
    // 2. 打开管道文件   open（）；
    int fd_fifo = open( FIFO_PATH , O_RDWR); // 如果管道只有读者、写者将阻塞在open函数中 ， 使用O_RDWR 则可以避免该阻塞出现
    if (-1 == fd_fifo)
    {
        perror("open fifo error");
        return -1 ;
    }
    
    // 3. 读取、写入数据 read/write
    int ret_val = write(fd_fifo , "Hello Even", 11 );
    if (ret_val <= 0 )
    {
        perror("write error");
        return -1 ;
    }
    printf("write success!\n");
    
    // 4. 关闭文件
    close(fd_fifo);
    
    return 0;
}
