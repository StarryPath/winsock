#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <iostream>
#include <sstream> 
#pragma comment(lib, "ws2_32.lib")  //���� ws2_32.dll
using namespace std;
int main() {
	//��ʼ��DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//�����׽���
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//���������������
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(1234);
	char buff[MAXBYTE] = { 0 };
	char recvbuff[MAXBYTE] = { 0 };
	sockaddr_in addr;
	int nLen = sizeof(addr);
	
	for (int i = 0; i < 100; i++)
	{
		
		stringstream sstr;
		
		sstr << i;
		string str = sstr.str();
		string aa = "hello world , and now you can begin the test hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhha";
		str = str + aa;
		
		
		
		const char*strr = str.c_str();
		Sleep(10);
		if (sendto(sock, strr, strlen(strr), 0, (sockaddr*)&sockAddr, sizeof(sockAddr)) <= 0)
		{
			printf("send failed !code:%d\n", WSAGetLastError());
			closesocket(sock);
			WSACleanup();
			break;
		}
		if (strcmp(buff, "q") == 0)
		{
			printf("���˳�\n");
			break;
		}
		printf("%s\n", strr);
		
	}
	Sleep(1000);
	sendto(sock, "q", 1, 0, (sockaddr*)&sockAddr, sizeof(sockAddr));
	system("pause");
	return 0;
}