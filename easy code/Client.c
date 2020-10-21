#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
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
    Server.sin_port = htons(55000) ; // 需要把主机字节序的65000转换为网络字节序
    Server.sin_addr.s_addr = inet_addr("192.168.100.36"); // 把字符串的网络地址转换为点分二进制

    //配置服务器的IP地址等信息 
    struct sockaddr_in  my_addr;
    int addrlen = sizeof(struct sockaddr_in);
    my_addr.sin_family = AF_INET ; // 选择使用IPV4协议
    my_addr.sin_port = htons(65000) ; // 需要把主机字节序的65000转换为网络字节序
    my_addr.sin_addr.s_addr = inet_addr("192.168.100.36"); // 把字符串的网络地址转换为点分二进制
    char buf[] =
"\n⣠⣇\n⡙⣿⣤⣾⣿⣿⣿⣶⣄\n⣨⣿⣿⣿⣿⣿⣿⣿⣿⣿⣄⣠⡶⠋\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡶⠶⠒\n⢻⣿⡟⣾⡙⠧⣊⢚⣪⡊⣸⢻⣟⠶⣄\n⣀⣙⣷⠙⠀⠀⠀⢇⢽⣞⡏⣸⠋\n⠙⢛⣻⠟⠒⠀⠀⠀⠀⠀⣩⠋\n⣿⡏⠀⠀⠀⡗⢄⠀⡠⠛\n⠋⠀⠀⠀⠉⠙⢄⠉\n⢎⠑⣇⣸⠶⣧⣸⠶⡎⠑⣇";
    sendto(socket_fd, buf, sizeof(buf), MSG_NOSIGNAL, (struct sockaddr *)&Server, Server_addrlen);
    return 0;
}
