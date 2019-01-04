#include <stdio.h>
#include <winsock2.h>
#include <ctime>
#include <cstdio>
#include <windows.h>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll
char buff[MAXBYTE] = { 0 };
char recvbuff[MAXBYTE] = { 0 };
int recvstate;
HANDLE hThread1;
DWORD ThreadID1;
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
		if (recvstate == 10)//接收成功
		{
			printf("%s", recvbuff);
			send(int(clntSock), recvbuff, sizeof(recvbuff), 0);
		}
		
		if (strcmp("q", recvbuff) == 0)
		{
			closesocket(int(clntSock));
			break;
		}


		//关闭套接字
	}
}
int main() {

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

