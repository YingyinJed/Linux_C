#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    //1、打开文件
    //""双引号在参数传递时，会被当做char*

    int fd_test = -1;//定义一个变量存储打开状态
    fd_test = open("./test.c",O_WRONLY);//尝试打开文件

    //2、写入信息
    if(fd_test == -1)//判断打开状态
    {
        //打印错误信息
        printf("open file\n");
        return -1;//打开失败直接退出函数
    }
    char buffer[] = "What the fuck ?!";
    int ret = write(fd_test,buffer,sizeof(buffer));//"默认有一个字符串结束符"
    printf("write in %d words\n",ret);

    //3、关闭文件
    close(fd_test);
    /* code */
    return 0;
}
