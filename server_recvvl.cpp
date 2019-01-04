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
char buff[MAXBYTE] = { 0 };
char recvbuff[MAXBYTE] = { 0 };
char temp[10] = { 0 };
int recvstate;
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

	}
	return fixedlen;
}
int recvvl(SOCKET s, char * recvbuf, unsigned int recvbuflen)
{
	int iResult;//�洢����recv�����ķ���ֵ
	unsigned int reclen; //���ڴ洢����ͷ���洢�ĳ�����Ϣ
	//��ȡ���ձ��ĳ�����Ϣ
	iResult = recvn(s, recvbuf, sizeof(unsigned int));
	if (iResult != sizeof(unsigned int))
	{
		/*��������ֶ��ڽ���ʱû�з���һ���������ݾͷ��أ����ӹرգ���-1����������*/
		if (iResult == -1) {
			printf("���շ�������: %d\n", WSAGetLastError());
			return -1;
		}
		else {
			printf("���ӹر�\n");
			return 0;
		}
	}
	strcpy(temp, recvbuf);

	printf("temp:%s\n", temp);
	stringstream sstr(temp);
	sstr >> reclen;
	//ת�������ֽ�˳�������ֽ�˳��
	memset(recvbuff, '\0', sizeof(recvbuff));
	printf("reclen:%u\n ", reclen);
	if (reclen > recvbuflen)
	{
		//���recvbufû���㹻�Ŀռ�洢�䳤��Ϣ������ո���Ϣ�����������ش���
		printf("û���㹻�Ŀռ�");
		while (reclen > 0)
		{
			iResult = recvn(s, recvbuf, recvbuflen);
			if (iResult != recvbuflen)
			{
				//����䳤��Ϣ�ڽ���ʱû�з����㹻�����ݾͷ��أ����ӹرգ���-1����������
				if (iResult == -1) {
					printf("���շ�������: %d\n", WSAGetLastError());
					return -1;
				}
				else {
					printf("���ӹر�\n");
					return 0;
				}
			}
			reclen -= recvbuflen;
			//�������һ�����ݳ���
			if (reclen < recvbuflen)
				recvbuflen = reclen;
		}
		printf("�ɱ䳤�ȵ���Ϣ����Ԥ����Ľ��ջ���\r\n");
		return -1;
	}
	//���տɱ䳤��Ϣ
	iResult = recvn(s, recvbuf, reclen);
	if (iResult != reclen) {
		/*(�����Ϣ�ڽ���ʱû�з����㹻�����ݾͷ��أ����ӹرգ���-1����������*/
		if (iResult == -1) {
			printf("���շ�������: %d\n", WSAGetLastError());
			return -1;
		}
		else {
			printf("���ӹر�\n");
			return 0;
		}
	}
	return iResult;
}
void ThreadFun1(LPVOID clntSock)
{
	while (true)
	{

		recvstate = recvvl(int(clntSock), recvbuff, MAXBYTE);
		if (recvstate >0)//���ճɹ�
		{
			printf("recvbuff:%s\n", recvbuff);
			send(int(clntSock), temp, strlen(temp), 0);
			send(int(clntSock), recvbuff, strlen(recvbuff), 0);
			memset(recvbuff, '\0', sizeof(recvbuff));
		}

		if (strcmp("q", recvbuff) == 0)
		{
			closesocket(int(clntSock));
			break;
		}
	}
}
int main() {
	printf("%d", sizeof(unsigned int));
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

