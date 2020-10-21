#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>

//函数声明
void * func(void * arg);

//定义全局变量
int flag = 1 ;

int main(int argc, char const *argv[])
{
    pthread_t  TID ; //  创建线程
    pthread_create( &TID , NULL, func , NULL);
    //创建套接字
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0 );
    if (socket_fd == -1)
    {
        perror("socket err ");
        return -1 ;
    }
        struct sockaddr_in  Server;
    int Server_addrlen = sizeof(struct sockaddr_in);
    Server.sin_family = AF_INET ; // 选择使用IPV4协议
    Server.sin_port = htons(65532) ; // 需要把主机字节序的65000转换为网络字节序
    Server.sin_addr.s_addr = inet_addr("192.168.100.36"); // 把字符串的网络地址转换为点分二进制

    //配置服务器的IP地址等信息 
    struct sockaddr_in  my_addr;
    int addrlen = sizeof(struct sockaddr_in);
    my_addr.sin_family = AF_INET ; // 选择使用IPV4协议
    my_addr.sin_port = htons(65531) ; // 需要把主机字节序的65000转换为网络字节序
    my_addr.sin_addr.s_addr = inet_addr("192.168.100.36"); // 把字符串的网络地址转换为点分二进制
    char buf[1024] ;
    printf("----------客户端准备就绪--------------\n");
    while (1)
    {
        bzero(buf , 1024); //  清空用户缓冲区
        fgets( buf , 1024 , stdin); //  每一个程序运行起来都有三个资源文件
                //       0  stdin    1  stdout   2 stderr
                //        键盘        屏幕/终端   屏幕/终端
        sendto(socket_fd, buf, sizeof(buf), MSG_NOSIGNAL, (struct sockaddr *)&Server, Server_addrlen);
        printf("Send:%s\n",buf);
        sleep(5);
    }   
    return 0;
}

void * func(void * arg)
{
    //创建套接字 （购买手机）
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0 );
    if (-1 == socket_fd)
    {
        perror("socket err ");
    } 
    //配置服务器的IP地址等信息 (购买手机卡)
    // struct sockaddr_in
    // {
    //     u_short sin_family; // 地址族
    //     u_short sin_port; // 端口
    //     struct in_addr sin_addr; // IPV4 地址
    //     char sin_zero[8];
    // };
    struct sockaddr_in  my_addr;
    int addrlen = sizeof(struct sockaddr_in);
    my_addr.sin_family = AF_INET ; // 选择使用IPV4协议
    my_addr.sin_port = htons(65534) ; // 需要把主机字节序的65000转换为网络字节序
    my_addr.sin_addr.s_addr = inet_addr("192.168.100.36"); // 把字符串的网络地址转换为点分二进制
    //绑定 （把手机与手机卡进行绑定）
    int ret_val =  bind(socket_fd , (struct sockaddr *)&my_addr, addrlen);
    if (-1 == ret_val)
    {
        perror("bind error");
    }
    //等待来信
    char msg[1024] = {0};
    struct sockaddr_in  from_addr;
    printf("----------服务器准备就绪--------------\n");
    while(1)
    {
        ret_val = recvfrom(socket_fd , msg, 1024 , 0 , (struct sockaddr *)&from_addr, &addrlen);
        if (ret_val > 0 )
        {
             printf("接收到:%s\n来自端口:%d\n在地址:%s\n",msg,ntohs(from_addr.sin_port),inet_ntoa(from_addr.sin_addr));
        }
    }
}