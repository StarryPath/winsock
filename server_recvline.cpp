#include <stdio.h>
#include <winsock2.h>
#include <ctime>
#include <cstdio>
#include <windows.h>
#pragma comment (lib, "ws2_32.lib")  //���� ws2_32.dll
char buff[MAXBYTE] = { 0 };
char recvbuff[MAXBYTE] = { 0 };
int recvstate;
HANDLE hThread1;
DWORD ThreadID1;
void ThreadFun1(LPVOID clntSock)
{
	while (true)
	{
		recvstate=recv(int(clntSock), recvbuff, MAXBYTE, NULL);
		printf("%s ", recvbuff);
		if (strcmp("q", recvbuff) == 0)
		{
			closesocket(int(clntSock));
			break;
		}
		else
		{ 
			
			int send_num=send(int(clntSock), recvbuff, sizeof(recvbuff), 0);
			
		}
		
		//�ر��׽���
	}
}
int main() {

	//��ʼ�� DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//�����׽���
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//���׽���
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
	sockAddr.sin_family = PF_INET;  //ʹ��IPv4��ַ
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //�����IP��ַ
	sockAddr.sin_port = htons(1234);  //�˿�
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

	//�������״̬
	listen(servSock, 20);

	//���տͻ�������
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);

	while (true)
	{
		SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
		hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFun1, (void*)clntSock, 0, &ThreadID1);
		printf("���߳�");
	}
	closesocket(servSock);

	//��ֹ DLL ��ʹ��
	WSACleanup();

	return 0;
}

