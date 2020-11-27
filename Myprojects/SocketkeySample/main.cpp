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

void main(int argc, char* argv[])
{
	// 서버 IP 및 포트 입력 받아서 접속 
	//if (argc != 3)
	//{
	//	return;
	//}

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	const char* ipAddress = "1912.168.0.155"; // argv[1];
	unsigned short iPort = 10000; // atoi(argv[2]);

	while (1)
	{
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

		int optval = 1;
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) != 0)
		{
			return;
		}

		//linger optLinger;
		//int iLingerLen = sizeof(optLinger);
		//optLinger.l_onoff = 1;
		//optLinger.l_linger = 1000;
		//if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optLinger, iLingerLen) != 0)
		//{
		//	return;
		//}

		SOCKADDR_IN sa;
		USHORT jValue = 10000;
		sa.sin_family = AF_INET;
		sa.sin_addr.s_addr = inet_addr(ipAddress);
		sa.sin_port = htons(iPort);

		int iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
		if (iRet == SOCKET_ERROR)
		{
			return;
		}



		//iRet = listen(sock, SOMAXCONN);
		//SOCKADDR_IN clientAddr;
		//int len = sizeof(clientAddr);
		//SOCKET Client = accept(sock, (SOCKADDR*)&clientAddr, &len);
		//printf("\n%s : %d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

		int iCount = 0;
		clock_t s = clock();
		clock_t e = clock();
		sMsg msg;
		bool bConnect = true;
		int iRecvSize = 0;
		int iSendSize = 0;
		int iPacketSize = sizeof(sMsg);
		//vector<sMsg> recvlist;
		char recvBuf[10000] = { 0, };

		while (e - s < 10000 && bConnect)
		{
			while (iSendSize < iPacketSize)
			{
				memset(&msg, 0, sizeof(msg));
				strcpy_s(msg.buffer, 32, "안녕하세요....");
				msg.iCnt = iCount;
				char recvBuf[3001] = { 0, };
				clock_t t1 = clock();
				iSendSize = send(sock, (char*)&msg, iPacketSize - iSendSize, 0);
				if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
			}
			memset(recvBuf, 0, sizeof(char) * 10000);
			while (iRecvSize < iPacketSize && bConnect)
			{

				iRecvSize += recv(sock, recvBuf, iPacketSize - iRecvSize, 0);
				if (iRecvSize == 0 || iRecvSize == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
				if (sizeof(sMsg) == iRecvSize)
				{
					memcpy(&msg, recvBuf, sizeof(msg));
					printf("\n%d : %s", msg.iCnt, msg.buffer);
				}
			}
			iRecvSize = 0;
			iSendSize = 0;
			e = clock();
			clock_t t = e - s;
			iCount++;
		}
		closesocket(sock);
		Sleep(2000);
	}
	WSACleanup();
}