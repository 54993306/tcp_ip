
// 基于 udp 的回声服务器
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char* message);

int main(int argc , char* argv[])
{
    int serv_sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t clnt_adr_sz;

    struct sockaddr_in serv_adr , clnt_adr;

    if(argc != 2){
        printf("usage : %s <port> \n" , argv[0]);    
        exit(1);
    }

    serv_sock = socket(PF_INET , SOCK_DGRAM , 0);  //协议族没有变,传输数据的格式改为数据报格式
    if(serv_sock == -1)
        error_handling("sock() error");
    
    memset(&serv_adr , 0 , sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock , (struct sockaddr*)&serv_adr , sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    
    while(1)  //服务器端不会跳出这个循环,不能被关闭
    {
        clnt_adr_sz = sizeof(clnt_adr);
        str_len = recvfrom(serv_sock , message , BUF_SIZE , 0 , (struct sockaddr*)&clnt_adr , &clnt_adr_sz);
        sendto(serv_sock , message , str_len , 0 , (struct sockaddr*)&clnt_adr , clnt_adr_sz );
        // 服务器的字符串数组没有被清理,下一次的消息如果比上一次的内容少则会有残留内容
        printf("recv client message : %s \n" , message);  
    }
    close(serv_sock);
    return 0;
}

void error_handling(char * message)
{
    fputs(message , stderr);
    fputc('\n',stderr);
    exit(1);
}