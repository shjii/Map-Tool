#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>
#include <Time.h>
#include <vector>
#include <conio.h>
#pragma comment (lib, "ws2_32.lib")
using namespace std;
struct sMsg
{
	int iCnt;
	char buffer[3000];
};

void main(int argc, char* argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	const char* ipAddress = "192.168.0.155"; // argv[1];
	unsigned short iPort = 10000; // atoi(argv[2]);
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	int optval = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) != 0)
	{
		return;
	}
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
	//넌블로킹 소켓으로 전환
	unsigned long iMode = 1;
	ioctlsocket(sock, FIONBIO, &iMode);

	int iCount = 0;
	sMsg msg;

	int iRecvSize = 0;
	int iSendSize = 0;
	int iPacketSize = sizeof(sMsg);
	char recvBuf[10000] = { 0, };
	char sendBuf[10000] = { 0, };
	bool bConnect = true;
	bool bRecvData = false;
	int iMsgLength = 0;
	while (bConnect)
	{
		if (_kbhit() != 0)
		{
			int iValue = _getche();
			int ilen = strlen(sendBuf);
			if (ilen == 0 && iValue == '\r')
			{
				break;
			}
			if (iValue == '\r')
			{
				memset(&msg, 0, sizeof(msg));
				strcpy_s(msg.buffer, sendBuf);
				msg.iCnt = iCount++;

				iMsgLength = 0;
				while (iSendSize < iPacketSize)
				{
					char recvBuf[3001] = { 0, };
					clock_t t1 = clock();
					iSendSize += send(sock, (char*)&msg, iPacketSize - iSendSize, 0);
					if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
					{
						if (WSAGetLastError() != WSAEWOULDBLOCK)
						{
							bConnect = false;
						}
						break;
					}
				}
				memset(sendBuf, 0, sizeof(char) * 10000);
			}
			else
			{
				sendBuf[iMsgLength++] = iValue;
			}
		}
		memset(recvBuf, 0, sizeof(char) * 10000);
		bRecvData = true;
		while (iRecvSize < iPacketSize && bRecvData)
		{
			iRecvSize += recv(sock, recvBuf, iPacketSize - iRecvSize, 0);
			if (iRecvSize == 0)
			{
				bConnect = false;
				break;
			}
			if (iRecvSize == SOCKET_ERROR)
			{
				bRecvData = false;
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					bConnect = false;
					break;
				}				
			}
			if (sizeof(sMsg) == iRecvSize)
			{
				memcpy(&msg, recvBuf, sizeof(msg));
				printf("\n%d : %s", msg.iCnt, msg.buffer);
				bRecvData = false;
			}
		}
		iRecvSize = 0;
		iSendSize = 0;
	}
	shutdown(sock, SD_SEND);
	closesocket(sock);
	WSACleanup();
}