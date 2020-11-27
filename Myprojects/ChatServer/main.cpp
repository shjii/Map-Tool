#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>
#include <Time.h>
#include <vector>
#include <list>
#include <string>
#pragma comment (lib, "ws2_32.lib")
using namespace std;
struct sMsg
{
	int iCnt;
	char buffer[3000];
};

struct sUser //나중에 구조체로 MSG 날림 
{
	SOCKET sock;
	SOCKADDR_IN addr;
	vector<sMsg> msgList;
};


void Error(const CHAR* msg = 0, const char* IpData = 0)
{
	LPVOID* IpMsg = 0;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(CHAR*)&IpMsg, 0, NULL);
	string szBuffer = (IpData != nullptr) ? IpData : "";
	szBuffer += "\n";
	szBuffer += (CHAR*)IpMsg;

	MessageBoxA(NULL, szBuffer.c_str(), msg, MB_ICONERROR);
	LocalFree(IpMsg);
}

void Check(int iRet, int line)
{
	if (iRet == SOCKET_ERROR)
	{
		CHAR szBuffer[256] = { 0 , };
		sprintf_s(szBuffer, "%s[%d]", __FILE__, line);
		Error("ERROR", szBuffer);
		exit(1);
	}
}

void main()
{
	WSADATA wsa;
	int iRet;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		Error("socket");
		return;
	}
	int sockType;
	int sockTypeLen = sizeof(int);

	int iSendBuffer = 100000;
	setsockopt(sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&iSendBuffer, sockTypeLen);

	getsockopt(sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&sockType, &sockTypeLen);
	printf("SendBuffer=%d\n", sockType);
	getsockopt(sock, SOL_SOCKET, SO_RCVBUF,
		(char*)&sockType, &sockTypeLen);
	printf("RecvBuffer=%d\n", sockType);

	int optval = 1;
	Check(setsockopt(sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&optval, sizeof(optval)), __LINE__);
	linger optLinger;
	int iLingerLen = sizeof(optLinger);
	optLinger.l_onoff = 1; // 0이면 바로 종료
	optLinger.l_linger = 1000;
	if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&optLinger, iLingerLen) != 0)
	{
		Error("SO_LINGER");
		return;
	}
	SOCKADDR_IN sa;
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY); // 자신의 모든 IP 값 사용
	sa.sin_port = htons(10000);

	Check(bind(sock, (SOCKADDR*)&sa, sizeof(sa)), __LINE__ );

	iRet = listen(sock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)
	{
		Error("listen");
		return;
	}
	bool bRecvData = false;
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);

	//넌블로킹 소켓으로 전환
	unsigned long iMode = 1;
	ioctlsocket(sock, FIONBIO, &iMode);

	list<SOCKET> userlist;

	while (1)
	{
		SOCKET Client = accept(sock, (SOCKADDR*)&clientAddr, &len);
		if (Client == INVALID_SOCKET)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				break;
			}
		}
		else
		{
			userlist.push_back(Client);
			printf("\n 접속 -> %s : %d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
		}
		
		int iCount = 0;
		bRecvData = true;

		sMsg msg;
		char recvBuf[10000] = { 0, };
		char* pRecv = recvBuf;
		int iRecvSize = 0;
		int iSendSize = 0;
		int iPacketSize = sizeof(sMsg);

		list<SOCKET>::iterator iter;
		for (iter = userlist.begin(); iter != userlist.end();)
		{
			bool bDelete = false;
			bRecvData = true;
			while (iRecvSize < iPacketSize && bRecvData)
			{
				iRecvSize += recv(*iter, recvBuf, iPacketSize - iRecvSize, 0);
				if (iRecvSize == 0 || iRecvSize == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						bDelete = true;
						bRecvData = false;
						shutdown(*iter, SD_SEND);
						closesocket(*iter);
						iter = userlist.erase(iter);
					}
					bRecvData = false;
					break;
				}
			}
			while (iSendSize < iPacketSize && bRecvData)
			{
				memcpy(&msg, recvBuf, sizeof(sMsg));
				iSendSize += send(*iter, recvBuf, iPacketSize - iSendSize, 0);
				if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
				{
						if (WSAGetLastError() != WSAEWOULDBLOCK)
						{
							bDelete = true;
							bRecvData = false;
							shutdown(*iter, SD_SEND);
							closesocket(*iter);
							iter = userlist.erase(iter);
							continue;
						}
						break;
					}
				}
				iRecvSize = 0;
				iSendSize = 0;
				if (bDelete == false)
				{
					iter++;
				}
			}
		}
	closesocket(sock);
	WSACleanup();
}