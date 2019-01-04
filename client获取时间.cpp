#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")  //���� ws2_32.dll

int main() {
	//��ʼ��DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//�����׽���
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//���������������
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(1234);
	connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

	//���շ��������ص�����
	char szBuffer[MAXBYTE] = { 0 };
	int num_recv;
	
	do{
		num_recv = recv(sock, szBuffer , MAXBYTE, NULL);
		if (num_recv > 0)
			printf("%s\n", szBuffer);
		else {
			if (num_recv == 0)
				printf("Connection closed\n");
			else
				printf("recv failed with error: %d\n", WSAGetLastError());
		}
	}while (num_recv>0);
	//�ر��׽���
	closesocket(sock);

	//��ֹʹ�� DLL
	WSACleanup();

	system("pause");
	return 0;
}
