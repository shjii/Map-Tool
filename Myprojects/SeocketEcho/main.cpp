#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>
#include <Time.h>
#include <vector>
#pragma comment (lib, "ws2_32.lib")
using namespace std;
struct sMsg
{
	int iCnt;
	char buffer[3000];
};

void main()
{
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("192.168.0.155");
	sa.sin_port = htons(10000);
	
	int iRet = bind(sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	iRet = listen(sock, SOMAXCONN);
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);
	SOCKET Client = accept(sock, (SOCKADDR*)&clientAddr, &len);
	printf("\n%s : %d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
	int iCount = 0;
	clock_t s = clock();
	clock_t e = clock();
	sMsg msg;

	vector<sMsg> recvlist;
	char recvBuf[10000] = { 0, };
	char* pRecv = recvBuf;
	int iRecvSize = 0;
	int iPacketSize = sizeof(sMsg);
	while (e - s < 1000)
	{
		iRecvSize += recv(Client, recvBuf, iPacketSize - iRecvSize, 0);
		if (iRecvSize == 0 || iRecvSize == SOCKET_ERROR)
		{
			break;
		}
		if (sizeof(sMsg) == iRecvSize)
		{
			memcpy(&msg, recvBuf, sizeof(sMsg));
			iRet = send(Client, (char*)&msg, iRecvSize, 0);
			if (iRet == 0 || iRet == SOCKET_ERROR)
			{
				break;
			}
			recvlist.push_back(msg);
			printf("\n%d : %s", msg.iCnt, msg.buffer);
			iRecvSize = 0;
		}
		else
		{
			break;
		}
	}
	closesocket(sock);
	closesocket(Client);
	WSACleanup();
	recvlist.clear();
}