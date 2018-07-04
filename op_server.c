
// 实现可计算客户端传输操作数据的服务器端
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define OPSZ 4

void error_handling(char* message);
int calculate(int opnum , int opnds[] , char oprator);

int main(int argc , char* argv[])
{
    int serv_sock , clnt_sock; // 服务器端套接字 , 与客户端连接受理后生成的套接字
    char opinfo[BUF_SIZE];
    int result , opnd_cnt , i;
    int recv_cnt , recv_len;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc != 2)
    {
        printf("usage : %s <port> \n" , argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET , SOCK_STREAM , 0);  //声明一个 tcp 的套接字

    memset(&serv_adr , 0 , sizeof(serv_adr));       // 初始化套接字地址结构体
    serv_adr.sin_family = AF_INET;                  //AF 表示ADDRESS FAMILY 地址族，PF 表示PROTOCOL FAMILY 协议族，但这两个宏定义是一样的，所以使用哪个都没有关系。
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock , (struct sockaddr*) &serv_adr , sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    
    if(listen(serv_sock , 5) == -1)
        error_handling("listen() error");
    
    clnt_adr_sz = sizeof(clnt_adr);

    for(i=0 ; i < 5 ; i++)
    {
        opnd_cnt = 0;
        clnt_sock = accept(serv_sock , (struct sockaddr*)&clnt_adr , &clnt_adr_sz);
        if (clnt_sock == -1) 
            error_handling(" accept() error ");
        else
            printf("Connected client %d \n" , i+1 );
            
        read(clnt_sock , &opnd_cnt , 1); // 读取第一个字节,读取操作数,存储到 open_cnt 中
        
        recv_len = 0;
        while((opnd_cnt * OPSZ +1) > recv_len) // 操作数个数本身占1个字节,其他的操作数每个4字节
        {
            recv_cnt = read( clnt_sock , &opinfo[recv_len] , BUF_SIZE - 1); // 会把 clnt_sock 指定的套接字,中读取 BUF_SIZE 个字节到 opinfo 当中
            recv_len = recv_cnt;
        }
        
        result = calculate(opnd_cnt , (int*)opinfo , opinfo[recv_len - 1]);  //最后一个字符为运算操作符
        write(clnt_sock , (char*)&result , sizeof(result));                  //将计算结果发送给客户端
        printf("operation result : %d \n" , result);

        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}

int calculate(int opnum , int opnds[] , char op)
{
    int result = opnds[0] , i;  // 第一个被操作数默认为结果
    switch(op)
    {
        case '+':
            for(i=1 ; i < opnum ; i++)
                result += opnds[i];
            break;
        case '-':
            for(i=1 ; i < opnum ; i++)
                result -= opnds[i];
            break;
        case '*':
            for(i=1 ; i < opnum ; i++)
                result *= opnds[i];
            break;
    }
    return result;
}

void error_handling(char* message)
{
    fputs(message , stderr);
    fputc('\n' , stderr);
    exit(1);
}