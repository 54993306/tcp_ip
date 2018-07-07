#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

// the class learn to uecho_client.c

// study udp socket in connecteda

void error_handling(char* message);

int main(int argc , char* argv[])
{
    int sock ;
    char message[BUF_SIZE];
    int str_len;
    socklen_t adr_sz;
    struct sockaddr_in serv_adr , form_adr;

    if(argc != 3)
    {
        printf("usage : %s <ip> <port> \n" , argv[0]);
        exit(1);
    }

    sock = socket(PF_INET , SOCK_DGRAM , 0);    //create udp socket
    if(sock == -1)
        error_handling("socket() error");
    
    memset(&serv_adr , 0 , sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    // turn to connected udp socket , big up data transmit speed, and can use write and read functions
    connect(sock , (struct sockaddr*)&serv_adr , sizeof(serv_adr));  // set the socket is connected

    while(1)
    {
        fputs("insert message(q to quit) : ",stdout);
        fgets(message , sizeof(message) , stdin);
        if(!strcmp(message , "q\n") || !strcmp(message , "Q\n"))
            break;
        /*
            // old type
            sendto(sock , message , strlen(message) , 0 , (struct sockaddr*)&serv_adr , sizeof(serv_adr));
        */
        write(sock , message , strlen(message));  // connected type udp socket , can user write function
        /*
            //old type
            adr_sz = sizeof(form_adr);
            str_len = recvform(sock , message , BUF_SIZE , 0 , (struct sockaddr*)&form_adr , &adr_sz);
        */
        str_len = read(sock , message , sizeof(message) - 1);

        message[str_len] = 0;
        printf("message form server : %s " , message);
    }
    close(sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message , stderr);
    fputc('\n' , stderr);
    exit(1);
}