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
unsigned long m_size;
int S_time;
int re;

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

	printf("输入接收缓冲区大小：\n");
	scanf("%ul",& m_size);
	setsockopt(servSock, SOL_SOCKET, SO_RCVBUF, (char*)&m_size, sizeof(ULONG));
	printf("输入接收速度：\n");
	scanf("%d", &S_time);
	int count = -1;
	int error;
	while (true)
	{
		Sleep(S_time);
		re=recvfrom(servSock, recvbuff, MAXBYTE, 0, (sockaddr*)&addr, &nLen);
		//sendto(servSock, strcat(str, recvbuff), MAXBYTE, 0, (sockaddr*)&addr, sizeof(addr));
		//error = WSAGetLastError();
		//printf("%d\n", error);
		
		if (re > 0)
			count++;
		printf("%s\n", recvbuff);
		if (strcmp(recvbuff, "q") == 0)
			break;
		memset(recvbuff, '\0', sizeof(recvbuff));
	}
	printf("丢包率为：%f", 1.0-count*0.01);
	closesocket(servSock);

	//终止 DLL 的使用
	WSACleanup();
	system("pause");
	return 0;
}

