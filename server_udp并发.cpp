#include <stdio.h>
#include <iostream>
#include <sstream> 
#include <winsock2.h>
#include <ctime>
#include <cstdio>
#include <windows.h>
#pragma comment (lib, "ws2_32.lib")  //���� ws2_32.dll

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
		memset(&childaddr, 0, sizeof(childaddr));  //ÿ���ֽڶ���0���
		childaddr.sin_family = AF_INET;  //ʹ��IPv4��ַ
		childaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //�����IP��ַ
		childaddr.sin_port = htons(0);  //�˿�
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

	
	int nLen = sizeof(addr);

	while (true)
	{
		printf("������");
		recvfrom(servSock, recvbuff, MAXBYTE, 0, (sockaddr*)&addr, &nLen);

		hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFun1, NULL, 0, &ThreadID1);
		
		
	}
	closesocket(servSock);

	//��ֹ DLL ��ʹ��
	WSACleanup();

	return 0;
}

