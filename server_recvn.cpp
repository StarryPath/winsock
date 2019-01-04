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
int recvn(SOCKET s, char * recvbuf, unsigned int fixedlen)
{
	int iResult; //�洢����recv�����ķ���ֵ
	int cnt; //����ͳ������ڹ̶����ȣ�ʣ������ֽ���δ����
	cnt = fixedlen;
	while (cnt > 0) {

		iResult = recv(s, recvbuf, cnt, 0);
		if (iResult < 0) {
			//���ݽ��ճ��ִ��󣬷���ʧ��
			printf("���շ�������: %d\n", WSAGetLastError());
			return -1;
		}
		if (iResult == 0) {
			//�Է��ر����ӣ������ѽ��յ���С��fixedlen���ֽ���
			printf("���ӹر�\n");
			return fixedlen - cnt;
		}
		printf("���յ����ֽ���: %d\n", iResult);
		//���ջ���ָ������ƶ�
		recvbuf += iResult;
		//����cntֵ
		cnt -= iResult;
		if(cnt>0)
		send(s, "length not enough", 18, 0);
	}
	return fixedlen;
}
void ThreadFun1(LPVOID clntSock)
{
	while (true)
	{

		recvstate = recvn(int(clntSock), recvbuff, 10);
		if (recvstate == 10)//���ճɹ�
		{
			printf("%s", recvbuff);
			send(int(clntSock), recvbuff, sizeof(recvbuff), 0);
		}
		
		if (strcmp("q", recvbuff) == 0)
		{
			closesocket(int(clntSock));
			break;
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

	}
	closesocket(servSock);

	//��ֹ DLL ��ʹ��
	WSACleanup();

	return 0;
}

