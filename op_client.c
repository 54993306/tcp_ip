
// 实现计算操作数的客户端
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4   //每个操作数所占字节数

void error_handling(char* message);

int main(int argc , char* argv[])
{
    int sock ;
    char opmsg[BUF_SIZE];  //为收发数据准备的存储空间
    int result , opnd_cnt , i ;
    struct sockaddr_in serv_adr;
    
    if(argc != 3)
    {
        printf(" usage : %s <IP> <port> \n" , argv[0]);
        exit(1);
    }

    sock = socket( PF_INET , SOCK_STREAM , 0 );
    if(sock == -1)
        error_handling("sock() error");
    
    memset( &serv_adr , 0 , sizeof(serv_adr) );
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect( sock , (struct sockaddr*) &serv_adr , sizeof(serv_adr)) == -1)
        error_handling("connect() error");
    else
        printf("connect.........");

    fputs("Operand count : " , stdout);
    scanf("%d" , &opnd_cnt);      //阻塞至有玩家输入操作数为止
    opmsg[0] = (char)opnd_cnt;   //第一个字符为操作数

    for(i=0; i< opnd_cnt ; i++) //根据操作数的个数,让用户循环输入相应的操作数信息
    {
        printf("Openrand %d : " , i+1);
        scanf("%d" , (int*)&opmsg[i*OPSZ + 1]); // 每4个字节存储一个操作数,要转化成 int* 类型
    }
    fgetc(stdin);
    fputs("Operator : " , stdout);
    scanf("%c" , &opmsg[opnd_cnt * OPSZ + 1]);  //输入操作符信息
    
    write(sock , opmsg , opnd_cnt*OPSZ+2);  // 将操作信息写入服务器端 socket

    read(sock , &result , RLT_SIZE);        // 从服务器端 socket 读取4字节的计算返回值

    printf("operation result : %d \n" , result);
    
    close(sock);

    return 0;
}

void error_handling(char * message)
{
    fputs(message , stderr);
    fputc('\n',stderr);
    exit(1);        //一旦错误则退出当前程序
}