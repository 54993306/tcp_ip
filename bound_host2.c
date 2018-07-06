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
    int sock ;
    char message1[] = "HI !";
    char message2[] = "I m another udp host!";
    char message3[] = "nice to meet you";

    struct sockaddr_in you_adr;
    if(argc != 3)
    {
        printf("usage : %s <IP> <port> \n" , argv[0]);
        exit(1);
    }

    sock = socket(PF_INET , SOCK_DGRAM , 0);
    if(sock == -1)
        error_handling("sock() error");
    
    memset(&you_adr , 0 , sizeof(you_adr));
    you_adr.sin_family = AF_INET;
    you_adr.sin_addr.s_addr = inet_addr(argv[1]);
    you_adr.sin_port = htons(atoi(argv[2]));    // if input error port then por can't show error buf it can't link to server

    sendto(sock , message1 , sizeof(message1) , 0 , (struct sockaddr*)&you_adr , sizeof(you_adr));
    sendto(sock , message2 , sizeof(message2) , 0 , (struct sockaddr*)&you_adr , sizeof(you_adr));
    sendto(sock , message3 , sizeof(message3) , 0 , (struct sockaddr*)&you_adr , sizeof(you_adr));

    close(sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message , stderr);
    fputc('\n' , stderr);
    exit(1);
}