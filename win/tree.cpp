// tree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#include "winsock2.h"
#include <windows.h>
int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA data;
	WSAStartup((MAKEWORD(2,0)),&data);
	SOCKET sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (!sock)return false;
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(5203);
	bind(sock,(sockaddr*)&addr,sizeof(sockaddr_in));	//将套接字sock绑定到地址addr上
	listen(sock,1000);									//
	sockaddr_in recAddr;
	int len = sizeof(sockaddr_in);

	while(true)
	{
		SOCKET recSock = accept(sock,(sockaddr*)&recAddr,(int*)&len);	//监听到之后接收链接
		//unsigned long ul = 1;
		//int nRet = ioctlsocket(recSock, FIONBIO, (unsigned long*)&ul);
		//if (nRet == SOCKET_ERROR)
		//{
		//	closesocket(recSock);
		//	//LOG_ERROR("设置非阻塞模式失败");
		//	return false;
		//}
		while (recSock)
		{
			int to = send(recSock,"hello",strlen("hello")+1,0);
			if (to==6)
			{
				printf("send success\n");
			}
			char buff[1024];

			int ret = recv(recSock,buff,1024,0);
			if (ret>0)
			{
				printf("Receive success : %s",buff);
			}
		}
	}
	return 0;
}