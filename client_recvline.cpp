#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")  //���� ws2_32.dll
BOOL recvline(SOCKET S, char* buf)
{
	BOOL retval = TRUE; //����ֵ
	BOOL bLineEnd = FALSE;//һ�ж�ȡ����
	int nReadLen = 0; //�����ֽ���
	int nDataLen = 0; //���ݳ���

	while (!bLineEnd)
	{
		nReadLen = recv(S, buf, MAXBYTE, 0);
	;
		if (nReadLen == SOCKET_ERROR)
		{
			  
			
			printf("%d", WSAGetLastError());
			
			retval = FALSE;   // ��ȡ����ʧ��  
			break;              // ����ѭ��  
		}
		if (0 == nReadLen)
		{
			printf("nReadLen==0");
			retval = FALSE; //��ȡ����ʧ��
			break;
		}
		
		bLineEnd = TRUE;	
	}
	return retval;
}
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
	//���������������
	char buff[MAXBYTE] = {0};
	char recvbuff[MAXBYTE] = {0};
	
		printf("Please input a string to send:\n");
		while (true)
		{
			scanf("%s", &buff);
			
			
				if (send(sock, buff, sizeof(buff), 0) <= 0)
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
				if (!recvline(sock, recvbuff))
				{
					closesocket(sock);
					WSACleanup();
					break;
				}

				//��ӡ�յ��Ļ����ַ���
				printf("%s\n", recvbuff);
			}
		
	system("pause");
	return 0;
}

