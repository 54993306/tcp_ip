#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc , char *argv[])
{
    int serv_sock;  // 服务器端套接字 文件夹描述符
    int clnt_sock;      // 客户端的套接字文件描述符

    struct sockaddr_in serv_addr;   // 服务器端地址
    struct sockaddr_in clnt_addr;   // 客户端地址
    socklen_t clnt_addr_size;       // 地址长度?

    char message[] = "hello word";

    if(argc != 2)
    {
        printf("usage : %s <port>\n",argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0); // 创建服务器端套接字,返回值是套接字的文件描述符
    if(serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr)); // 初始化服务器地址值都为0
    serv_addr.sin_family = AF_INET;  // 是 IPv4 网络协议的套接字类型，AF_INET6 则是 IPv6 的；而 AF_UNIX 则是 Unix 系统本地通信。
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //转换过来就是0.0.0.0，泛指本机的意思，也就是表示本机的所有IP，多网卡的情况下，这个就表示所有网卡ip地址的意思。
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)  // 给套接字分配 ip 地址和端口号
        error_handling("bind() error");

    if(listen(serv_sock , 5)== -1)  //转换 socket 的状态为可接受连接的状态
        error_handling("listen() error");
    
    clnt_addr_size = sizeof(clnt_addr);

    clnt_sock = accept( serv_sock, (struct sockaddr*) &clnt_addr , &clnt_addr_size); // 受理连接请求,如果没有连接会一直阻塞在这个位置
    if(clnt_sock == -1)
        error_handling("accept() error");
    
    write(clnt_sock, message , sizeof(message));   // 像连接请求中写入数据

    close(clnt_sock);
    close(serv_sock);

    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n' , stderr);
    exit(1);
}
