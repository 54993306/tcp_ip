
//time-out 情况设置 port
// 让强制关闭的服务器可快速重启

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TRUE 1
#define FALSE 0

void error_handling(char* message);

int main(int argc , char* argv[])
{
    int serv_sock , clnt_sock;
    char message[30];
    int option , str_len ;
    socklen_t optlen , clnt_adr_sz;
    struct sockaddr_in serv_adr , clnt_adr;

    if(argc != 2)
    {
        printf("usage : %s <port> \n" , argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET , SOCK_STREAM , 0);
    if(serv_sock == -1)
        error_handling("sock() error");
    
    // 设置服务器在 time-out 时可以将端口分配给新的套接字
    optlen = sizeof(option);
    option = TRUE;
    setsockopt(serv_sock , SOL_SOCKET , SO_REUSEADDR , (void*)&option , optlen);

    // nagle 算法的状态和开启的方法
    // int opt_val = 1;   // 1 为设置禁用状态,大文件数据传输应禁止 nagle 算法以提升速率,小文件应开启
    // setsockopt(serv_sock , IPPROTO_TCP , TCP_NODELAY , (void*)&opt_val , sizeof(opt_val));

    // 查看 nagle 算法状态
    // int opt_val;
    // optlen = sizeof(opt_val);
    // getsockopt(serv_sock , IPPROTO_TCP , TCP_NODELAY , (void*)&opt_val , &optlen);

    memset(&serv_adr , 0 , sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    // bind 的最后一个参数是 socklen_t 对象,而 accept 最后一个参数是指针
    if(bind(serv_sock , (struct sockaddr*)&serv_adr , sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    
    if(listen(serv_sock , 5) == -1)
        error_handling("listen() error");
        
    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock , (struct sockaddr*)&clnt_adr , &clnt_adr_sz);

    while((str_len = read(clnt_sock , message , sizeof(message))) != 0)
    {
        write(clnt_sock , message , str_len);
        write(1,message , str_len);
    }

    close(clnt_sock);
    close(serv_sock);
    return 0; 
}

void error_handling(char* message)
{
    fputs(message , stderr);
    fputc('\n' , stderr);
    exit(1);
}