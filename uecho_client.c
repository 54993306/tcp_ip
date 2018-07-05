
// 基于 udp 的回声客户端
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc , char* argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t adr_sz;
    struct sockaddr_in serv_adr , form_adr;

    if(argc != 3)
    {
        printf("usage : %s <IP> <Port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET , SOCK_DGRAM , 0);   //udp 传输, 协议族还是保持 ipv4协议,数据传输方式改为数据报传输
    if(sock == -1)
        error_handling("sock() error");

    memset(&serv_adr , 0 , sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;   // 地址族没变
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    while(1)
    {
        fputs("Inset message (q to quit) : " , stdout);
        fgets(message , sizeof(message) , stdin);
        if(!strcmp(message , "q\n") || !strcmp(message , "Q\n") ) // 按 q 或 Q 加换行符则跳出循环
            break;

        sendto(sock , message , strlen(message) , 0 , (struct sockaddr*)&serv_adr , sizeof(serv_adr));
        adr_sz = sizeof(form_adr);
        str_len = recvfrom(sock , message , BUF_SIZE , 0 , (struct sockaddr*)&form_adr , &adr_sz );
        message[str_len] = 0;
        printf("Message form server : %s",message);
    }
    close(sock);
    return 0;
}

void error_handling(char * message)
{
    fputs(message , stderr);
    fputc('\n', stderr);
    exit(1);
}