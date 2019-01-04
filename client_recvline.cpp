#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
BOOL recvline(SOCKET S, char* buf)
{
	BOOL retval = TRUE; //返回值
	BOOL bLineEnd = FALSE;//一行读取结束
	int nReadLen = 0; //读入字节数
	int nDataLen = 0; //数据长度

	while (!bLineEnd)
	{
		nReadLen = recv(S, buf, MAXBYTE, 0);
	;
		if (nReadLen == SOCKET_ERROR)
		{
			  
			
			printf("%d", WSAGetLastError());
			
			retval = FALSE;   // 读取数据失败  
			break;              // 跳出循环  
		}
		if (0 == nReadLen)
		{
			printf("nReadLen==0");
			retval = FALSE; //读取数据失败
			break;
		}
		
		bLineEnd = TRUE;	
	}
	return retval;
}
int main() {
	//初始化DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建套接字
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//向服务器发起请求
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(1234);
	connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	//向服务器发送数据
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
					printf("已退出\n");
					break;
				}
				if (!recvline(sock, recvbuff))
				{
					closesocket(sock);
					WSACleanup();
					break;
				}

				//打印收到的回显字符串
				printf("%s\n", recvbuff);
			}
		
	system("pause");
	return 0;
}

