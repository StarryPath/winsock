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
		printf("主线程");
	}
	closesocket(servSock);

	//终止 DLL 的使用
	WSACleanup();

	return 0;
}

