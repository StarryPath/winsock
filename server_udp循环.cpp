#include <stdio.h>
#include <iostream>
#include <sstream> 
#include <winsock2.h>
#include <ctime>
#include <cstdio>
#include <windows.h>
#pragma comment (lib, "ws2_32.lib")  //���� ws2_32.dll

using namespace std;

char str[100] = "echo:";
char str2[100] = "echo:";
int main() {
	//��ʼ�� DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//�����׽���
	SOCKET servSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//���׽���
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
	sockAddr.sin_family = AF_INET;  //ʹ��IPv4��ַ
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //�����IP��ַ
	sockAddr.sin_port = htons(1234);  //�˿�
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));



	//���տͻ�������
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

	//��ֹ DLL ��ʹ��
	WSACleanup();

	return 0;
}

