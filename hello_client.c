#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);
// argv 是传入的参数数组
int main(int argc , char * argv[])
{
    int sock;   // 客户端套接字文件描述符
    struct sockaddr_in serv_addr;  // 服务器端地址
    char message[30];  //缓存消息的数组
    int str_len = 0; //读取消息的长度,如果不初始化会得到一个无法预测的值
    int idx = 0, read_len = 0;

    if(argc != 3)
    {
        printf("usage : %s <IP> <Port> \n", argv[0] );
        exit(1);
    }

    sock = socket(PF_INET , SOCK_STREAM , 0);
    if(sock == -1)
        error_handling("sock() error ");

    memset(&serv_addr , 0 , sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock , (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) // 向固定 ip 地址的套接字发出连接请求
        error_handling("connect() error");
    
    // str_len = read(sock , message , sizeof(message) - 1);  //从连接的 socket 中读取出数据
    while( read_len = read(sock , &message[ idx++ ] , 1)){ // 为0时则跳出循环
        if( read_len == -1)
            error_handling("read() error");
        
        str_len += read_len;
    }

    printf("Function read call count: %d \n", str_len);
    
    printf("Message from server : %s \n",message);
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}