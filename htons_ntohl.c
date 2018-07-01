#include <stdio.h>
#include <arpa/inet.h>

// 本机字节序和网络字节序转换

int main(int argc, char const *argv[])
{
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x12345678;
    unsigned long net_addr;

    net_port = htons(host_port);  // htons 将 short 型的本机字节序转换为网络字节序 , 主要用于端口转换
    net_addr = htonl(host_addr);    // htonl 将 long 型的本机字节序转换为网络字节序, 主要用于地址转换

    printf("host port : %#x \n" , host_port);
    printf("host addr : %#x \n" , host_addr);
    printf("net port : %#x \n" , net_port);
    printf("net addr : %#x \n" , net_addr);

    return 0;
}
