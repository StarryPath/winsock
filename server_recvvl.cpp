#include <stdio.h>
#include <iostream>
#include <sstream> 
#include <winsock2.h>
#include <ctime>
#include <cstdio>
#include <windows.h>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll

using namespace std;
HANDLE hThread1;
DWORD ThreadID1;
char buff[MAXBYTE] = { 0 };
char recvbuff[MAXBYTE] = { 0 };
char temp[10] = { 0 };
int recvstate;
int recvn(SOCKET s, char * recvbuf, unsigned int fixedlen)
{
	int iResult; //存储单次recv操作的返回值
	int cnt; //用于统计相对于固定长度，剩余多少字节尚未接收
	cnt = fixedlen;
	while (cnt > 0) {

		iResult = recv(s, recvbuf, cnt, 0);
		
		if (iResult < 0) {
			//数据接收出现错误，返回失败
			printf("接收发生错误: %d\n", WSAGetLastError());
			return -1;
		}
		if (iResult == 0) {
			//对方关闭连接，返回已接收到的小于fixedlen的字节数
			printf("连接关闭\n");
			return fixedlen - cnt;
		}
		printf("接收到的字节数: %d\n", iResult);
		//接收缓存指针向后移动
		recvbuf += iResult;
		//更新cnt值
		cnt -= iResult;

	}
	return fixedlen;
}
int recvvl(SOCKET s, char * recvbuf, unsigned int recvbuflen)
{
	int iResult;//存储单次recv操作的返回值
	unsigned int reclen; //用于存储报文头部存储的长度信息
	//获取接收报文长度信息
	iResult = recvn(s, recvbuf, sizeof(unsigned int));
	if (iResult != sizeof(unsigned int))
	{
		/*如果长度字段在接收时没有返回一个整型数据就返回（连接关闭）或-1（发生错误）*/
		if (iResult == -1) {
			printf("接收发生错误: %d\n", WSAGetLastError());
			return -1;
		}
		else {
			printf("连接关闭\n");
			return 0;
		}
	}
	strcpy(temp, recvbuf);

	printf("temp:%s\n", temp);
	stringstream sstr(temp);
	sstr >> reclen;
	//转换网络字节顺序到主机字节顺序
	memset(recvbuff, '\0', sizeof(recvbuff));
	printf("reclen:%u\n ", reclen);
	if (reclen > recvbuflen)
	{
		//如果recvbuf没有足够的空间存储变长消息，则接收该消息并丢弃，返回错误
		printf("没有足够的空间");
		while (reclen > 0)
		{
			iResult = recvn(s, recvbuf, recvbuflen);
			if (iResult != recvbuflen)
			{
				//如果变长消息在接收时没有返回足够的数据就返回（连接关闭）或-1（发生错误）
				if (iResult == -1) {
					printf("接收发生错误: %d\n", WSAGetLastError());
					return -1;
				}
				else {
					printf("连接关闭\n");
					return 0;
				}
			}
			reclen -= recvbuflen;
			//处理最后一段数据长度
			if (reclen < recvbuflen)
				recvbuflen = reclen;
		}
		printf("可变长度的消息超出预分配的接收缓存\r\n");
		return -1;
	}
	//接收可变长消息
	iResult = recvn(s, recvbuf, reclen);
	if (iResult != reclen) {
		/*(如果消息在接收时没有返回足够的数据就返回（连接关闭）或-1（发生错误）*/
		if (iResult == -1) {
			printf("接收发生错误: %d\n", WSAGetLastError());
			return -1;
		}
		else {
			printf("连接关闭\n");
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
		if (recvstate >0)//接收成功
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
	//初始化 DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建套接字
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//绑定套接字
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	sockAddr.sin_family = PF_INET;  //使用IPv4地址
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
	sockAddr.sin_port = htons(1234);  //端口
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

	//进入监听状态
	listen(servSock, 20);

	//接收客户端请求
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);

	while (true)
	{
		SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
		hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFun1, (void*)clntSock, 0, &ThreadID1);

	}
	closesocket(servSock);

	//终止 DLL 的使用
	WSACleanup();

	return 0;
}

