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
unsigned long m_size;
int S_time;
int re;

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

	printf("������ջ�������С��\n");
	scanf("%ul",& m_size);
	setsockopt(servSock, SOL_SOCKET, SO_RCVBUF, (char*)&m_size, sizeof(ULONG));
	printf("��������ٶȣ�\n");
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
	printf("������Ϊ��%f", 1.0-count*0.01);
	closesocket(servSock);

	//��ֹ DLL ��ʹ��
	WSACleanup();
	system("pause");
	return 0;
}

