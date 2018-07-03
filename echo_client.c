#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char* message);

int main(int argc , char* argv[])
{   
    int sock ; 
    char message[BUF_SIZE];
    int str_len;
    int recv_len , recv_cnt;
    struct sockaddr_in serv_adr;

    if(argc != 3)
    {
        printf("Usage : %s <ip> <port> \n",argv[0]);
        exit(1);
    }

    sock = socket( PF_INET , SOCK_STREAM , 0);
    if(sock == -1)
        error_handling("sock() error \n");
    
    memset(&serv_adr , 0 , sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect( sock ,(struct sockaddr* ) &serv_adr , sizeof(serv_adr)) == -1)
        error_handling("connect() error \n");
    else
        puts("Connected .....");
        
    while(1)
    {
        fputs("input message(q to quite):" , stdout);
        fgets(message , BUF_SIZE , stdin);

        if( !strcmp(message , "q\n") || !strcmp(message , "Q\n"))  // input "q or Q" them quiet
            break;
        // 以这样的方式传输数据是会出现问题的,包体太大和太小都容易出现错误
        // write(sock , message , strlen(message));   // 会阻塞在这个位置直到有输入
        // str_len = read(sock , message , BUF_SIZE -1);
        // message[str_len] = 0 ;
        // printf("Message form server: %s " , message);

        // 修复客户端数据传输可能会一次性读取多条消息的问题,记录长度,长度不足则持续读取
        str_len = write( sock , message , strlen(message)); // 发送数据的长度
        recv_len = 0;
        while(recv_len < str_len)
        {
            recv_cnt = read( sock , &message[recv_len] , BUF_SIZE - 1);
            if(recv_cnt == -1)
                error_handling("read() error");
            recv_len += recv_cnt; //一直读取到发送出去的消息长度为止
        }
        message[recv_cnt] = 0;
        printf("message for server : %s \n" , message);
    }
    close(sock);
    return 0;
}

void error_handling(char * message)
{
    fputs(message , stderr);
    fputc('n' , stderr);
    exit(1);
}