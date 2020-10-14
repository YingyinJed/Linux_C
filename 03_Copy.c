#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
//如何使用主函数传参实现拷贝任意文件到文件文件名
int main(int argc, char const *argv[])
{
    //打开两个文件
    int fd_src = open("a.txt",O_RDONLY);
    if(fd_src == -1)
    {
        perror("open src error");
        return -1;
    }

    int fd_tag = open("b.txt",O_RDWR | O_CREAT,0611);
    if(fd_tag == -1)
    {
        perror("open tag error");
        return -1;
    }
    //读取源文件
    char buf[32];
    while(1)
    {
        int ret_val = read(fd_src,buf,32);

        //写入到目标文件
        write(fd_tag,buf,32);

        //判断读取是否到文件末尾
        if(ret_val < 32)
        {
            printf("read succed !\n");
            break;
        }
    }
    //关闭文件退出
    close(fd_src);
    close(fd_tag);
    /* code */
    return 0;
}
