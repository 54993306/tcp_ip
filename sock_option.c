
//套接字的多种可选项

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(char* message);

int main(int argc , char* argv[])
{
    int tcp_sock , udp_sock ;
    int sock_type;
    socklen_t optlen;
    int state;

    optlen = sizeof(sock_type);
    tcp_sock = socket(PF_INET , SOCK_STREAM , 0);
    printf("SOCK_STREAM : %d \n" , SOCK_STREAM);

    udp_sock = socket(PF_INET , SOCK_DGRAM , 0);
    printf("SOCK_DGRAM : %d \n" , SOCK_DGRAM);

    state = getsockopt(tcp_sock , SOL_SOCKET , SO_TYPE , (void*)&sock_type , &optlen);
    if(state)
        error_handling("getsockopt() error!");
    printf("Sockt type tcp : %d \n" , sock_type);

    state = getsockopt(udp_sock , SOL_SOCKET , SO_TYPE , (void*)&sock_type , &optlen);
    if(state)
        error_handling("getsockopt() error");
    printf("sock type udp : %d \n" , sock_type);

    int snd_buf , rcv_buf ;
    socklen_t len ;
    len = sizeof(snd_buf);

    state = getsockopt(tcp_sock , SOL_SOCKET , SO_SNDBUF , (void*)&snd_buf , &len);
    if(state)
        error_handling("getsockopt() snd error");
    printf("output buffer size : %d \n" , snd_buf);
    
    len = sizeof(rcv_buf);
    state = getsockopt(tcp_sock , SOL_SOCKET , SO_RCVBUF , (void*)&rcv_buf , &len);
    if(state)
        error_handling("getsockopt()  rcv error");
    printf("input buffer size : %d \n" , rcv_buf);

    int snd_size = 1024*3000 , rcv_size = 1024*3000;
    state = setsockopt(tcp_sock , SOL_SOCKET , SO_RCVBUF , (void*)&rcv_size , sizeof(rcv_size));
    if(state)
        error_handling("setopt rcv error");
    len = sizeof(rcv_size);
    state = getsockopt(tcp_sock , SOL_SOCKET , SO_RCVBUF , (void*)&rcv_size , &len);
    if(state)
        error_handling("getopt rcv size error");
    printf("set Input buffer size : %d \n" , rcv_size);

    state = setsockopt(tcp_sock , SOL_SOCKET , SO_SNDBUF , (void*)&snd_size , sizeof(snd_size));
    if(state)
        error_handling("setopt snd error");
    len = sizeof(snd_size);
    state = getsockopt(tcp_sock , SOL_SOCKET , SO_SNDBUF , (void*)&snd_size , &len);
    if(state)
        error_handling("get output buffer error");
    printf("set output buffer size : %d \n" , snd_size);

    return 0;
}

void error_handling(char* message)
{
    fputs(message , stderr);
    fputc('\n' , stderr );
    exit(1);
}