
#include <stdio.h>
#include <arpa/inet.h>


// 将字符串的 ip 地址转换为32位4字节的网络地址并转换为网络字节序
int main(int argc , char* argv[])
{
    char * addr1 = "1.2.3.4";
    char * addr2 = "1.2.3.256";  // 1字节能表示的最大值是255,当前的256是用来做错误测试用的

    unsigned long conv_addr = inet_addr(addr1);  //将字符串的 ip 地址转换成32位的网络字节序

    if(conv_addr == INADDR_NONE)
        printf(" error occurred \n");
    else
        printf("Network ordered integer addr : %#1x \n",conv_addr);

    conv_addr = inet_addr(addr2);

    if(conv_addr == INADDR_NONE)
        printf("addr2 error occurred \n");
    else
        printf("Network ordered integer addr : %#1x \n",conv_addr);
    
    return 0;
}


