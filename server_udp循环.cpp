#include <stdio.h>
#include <iostream>
#include <sstream> 
#include <winsock2.h>
#include <ctime>
#include <cstdio>
#include <windows.h>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll

using namespace std;

char str[100] = "echo:";
char str2[100] = "echo:";
int main() {
	//初始化 DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建套接字
	SOCKET servSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//绑定套接字
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	sockAddr.sin_family = AF_INET;  //使用IPv4地址
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
	sockAddr.sin_port = htons(1234);  //端口
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));



	//接收客户端请求
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);
	char recvbuff[MAXBYTE] = { 0 };
	sockaddr_in addr;
	int nLen = sizeof(addr);
	while (true)
	{
		
		recvfrom(servSock, recvbuff, MAXBYTE, 0, (sockaddr*)&addr, &nLen);
		sendto(servSock, strcat(str, recvbuff), MAXBYTE, 0, (sockaddr*)&addr, sizeof(addr));
		memset(recvbuff, '\0', sizeof(recvbuff));
		strcpy(str, str2);
	}
	closesocket(servSock);

	//终止 DLL 的使用
	WSACleanup();

	return 0;
}

