#include <stdio.h>
#include <iostream>
#include <sstream> 
#include <winsock2.h>
#include <ctime>
#include <cstdio>
#include <windows.h>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll

using namespace std;
HANDLE hThread1;
DWORD ThreadID1;
char recvbuff[MAXBYTE] = { 0 };
char buff[MAXBYTE] = { 0 };
char str[100] = "echo:";
char str2[100] = "echo:";
sockaddr_in addr;

void ThreadFun1()
{
		
		int n = sizeof(addr);
		SOCKET childsock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		sockaddr_in childaddr;
		memset(&childaddr, 0, sizeof(childaddr));  //每个字节都用0填充
		childaddr.sin_family = AF_INET;  //使用IPv4地址
		childaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
		childaddr.sin_port = htons(0);  //端口
		bind(childsock, (SOCKADDR*)&childaddr, sizeof(SOCKADDR));
		while (true)
		{
			strcpy(str, str2);
			sendto(childsock, strcat(str, recvbuff), sizeof(recvbuff), 0, (sockaddr*)&addr, sizeof(addr));
			
			memset(recvbuff, '\0', sizeof(recvbuff));
			strcpy(str, str2);
			
			int nn=recvfrom(childsock, recvbuff, MAXBYTE, 0, (sockaddr*)&addr, &n);

			
			printf("%s\n", strcat(str, recvbuff));
		}
}
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

	
	int nLen = sizeof(addr);

	while (true)
	{
		printf("主函数");
		recvfrom(servSock, recvbuff, MAXBYTE, 0, (sockaddr*)&addr, &nLen);

		hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFun1, NULL, 0, &ThreadID1);
		
		
	}
	closesocket(servSock);

	//终止 DLL 的使用
	WSACleanup();

	return 0;
}

