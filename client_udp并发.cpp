#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <iostream>
#include <sstream> 

#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
using namespace std;
int main() {
	//初始化DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建套接字
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//向服务器发起请求
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(1234);
	char buff[MAXBYTE] = { 0 };
	char recvbuff[MAXBYTE] = { 0 };
	sockaddr_in addr;
	int nLen = sizeof(addr);
	printf("Please input a string to send:\n");
	scanf("%s", &buff);

	//第一次
	if (sendto(sock, buff, strlen(buff), 0, (sockaddr*)&sockAddr, sizeof(sockAddr)) <= 0)
	{
		printf("send failed !code:%d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();

	}
	if (strcmp(buff, "q") == 0)
	{
		printf("已退出\n");

	}
	if (recvfrom(sock, recvbuff, MAXBYTE, 0, (sockaddr*)&addr, &nLen) <= 0)
	{
		closesocket(sock);
		WSACleanup();

	}
	printf("%s\n", recvbuff);
	memset(recvbuff, '\0', sizeof(recvbuff));
	while (true)
	{
		scanf("%s", &buff);


		//应答中的源端口号作为后续目的端口
		if (sendto(sock, buff, strlen(buff), 0, (sockaddr*)&addr, sizeof(addr)) <= 0)
		{
			printf("send failed !code:%d\n", WSAGetLastError());
			closesocket(sock);
			WSACleanup();
			break;
		}
		if (strcmp(buff, "q") == 0)
		{
			printf("已退出\n");
			break;
		}
		if (recvfrom(sock, recvbuff, MAXBYTE, 0, (sockaddr*)&addr, &nLen) <= 0)
		{
			closesocket(sock);
			WSACleanup();
			break;
		}

		//打印收到的回显字符串

		printf("%s\n", recvbuff);
		memset(recvbuff, '\0', sizeof(recvbuff));
	}

	system("pause");
	return 0;
}