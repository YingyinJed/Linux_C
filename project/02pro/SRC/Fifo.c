#include "Fifo.h"
/*
    函数名  ：Fifo_Init
    作  用  ：创建一个管道文件
    输入参数：fifopath为管道的创建路径
    返 回 值：该文件的文件编号
*/
int Fifo_Init(char* fifopath)
{
    // 0. 检查管道文件是否存在
    if (access( fifopath , F_OK))
    {
         // 1. 创建管道文件   mkfifo( );
        if(mkfifo(fifopath , 0666 ))
        {
            perror("mkfifo  error ");
            return -1 ;
        } 
    }
    
    // 2. 打开管道文件   open（）；
    int fd_fifo = open( fifopath , O_RDWR); // 如果管道只有读者、写者将阻塞在open函数中 ， 使用O_RDWR 则可以避免该阻塞出现
    if (-1 == fd_fifo)
    {
        perror("open fifo error");
        return -1 ;
    }
    return fd_fifo;
}
/*
    函数名  ：Mk_Fifo
    作  用  ：在管道内写入相应的命令
    输入参数：fifopath为管道的创建路径，command为写入管道的内容
    返 回 值：成功返回0，失败返回-1
*/
int Mk_Fifo(int fd_fifo,char * command)
{
    // 3. 读取、写入数据 read/write
    int ret_val = write(fd_fifo , command, strlen(command) );
    if (ret_val <= 0 )
    {
        perror("write error");
        return -1 ;
    }
    return 0;
}

