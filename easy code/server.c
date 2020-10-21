#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    //创建套接字 （购买手机）
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0 );
    if (-1 == socket_fd)
    {
        perror("socket err ");
        return -1 ;
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
    my_addr.sin_port = htons(65000) ; // 需要把主机字节序的65000转换为网络字节序
    my_addr.sin_addr.s_addr = inet_addr("192.168.100.36"); // 把字符串的网络地址转换为点分二进制

    //绑定 （把手机与手机卡进行绑定）
    int ret_val =  bind(socket_fd , (struct sockaddr *)&my_addr, addrlen);
    if (-1 == ret_val)
    {
        perror("bind error");
        return -1 ;
    }
    
    //等待来信
    char msg[1024] = {0};
    struct sockaddr_in  from_addr;

    while(1)
    {
        ret_val = recvfrom(socket_fd , msg, 1024 , 0 , (struct sockaddr *)&from_addr, &addrlen);
        if (ret_val > 0 )
        {
             printf("Msg:%s\nPort:%d\nAddr:%s\n",
                msg, 
                ntohs(from_addr.sin_port),
                inet_ntoa(from_addr.sin_addr));
        }
        
       
    
    }
    return 0;
}
