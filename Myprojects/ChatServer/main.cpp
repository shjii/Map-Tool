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

struct sUser
{
	SOCKET sock;
	SOCKADDR_IN addr;
	vector<sMsg> msgList;
	char recvBuf[10000];
};

int g_iPacketSize = sizeof(sMsg);
list<sUser> userlist;
SOCKET g_ListenSock;
vector<sMsg> g_recvPackt;

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

bool RecvUserData()
{
	int iRecvSize = 0;
	bool bRecvData = false;
	list<sUser>::iterator iter;
	for (iter = userlist.begin(); iter != userlist.end();)
	{
		sMsg msg;
		bool bDelete = false;
		bRecvData = true;
		while (iRecvSize < g_iPacketSize && bRecvData)
		{
			iRecvSize += recv(iter->sock, iter->recvBuf, g_iPacketSize - iRecvSize, 0);
			if (iRecvSize == 0 || iRecvSize == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					bDelete = true;
				}
				bRecvData = false;
				break;
			}
		}
		if (iRecvSize == g_iPacketSize)
		{
			memcpy(&msg, iter->recvBuf, sizeof(sMsg));
			g_recvPackt.push_back(msg);
		}
		iRecvSize = 0;
		if (bDelete == true)
		{
			shutdown(iter->sock, SD_SEND);
			closesocket(iter->sock);
			printf("\n 해제 -> %s : %d", inet_ntoa(iter->addr.sin_addr), ntohs(iter->addr.sin_port));
			iter = userlist.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	return true;
}
bool Broadcastting()
{
	int iSendSize = 0;
	list<sUser>::iterator iter;
	for (iter = userlist.begin(); iter != userlist.end();)
	{
		iSendSize = 0;
		bool bDelete = false;
		vector<sMsg>::iterator senditer;
		for (senditer = g_recvPackt.begin();
			senditer != g_recvPackt.end();
			senditer++)
		{
			while (iSendSize < g_iPacketSize)
			{
				iSendSize += send(iter->sock, iter->recvBuf, g_iPacketSize - iSendSize, 0);
				if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						bDelete = true;
					}
					break;
				}
			}
		}
		if (bDelete == true)
		{
			shutdown(iter->sock, SD_SEND);
			closesocket(iter->sock);
			iter = userlist.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	g_recvPackt.clear();
	return true;
}
bool InitNetwork()
{
	WSADATA wsa;
	int iRet;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	g_ListenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_ListenSock == INVALID_SOCKET)
	{
		Error("socket");
		return false;
	}
	int sockType;
	int sockTypeLen = sizeof(int);

	int iSendBuffer = 100000;
	setsockopt(g_ListenSock, SOL_SOCKET, SO_SNDBUF,
		(char*)&iSendBuffer, sockTypeLen);

	getsockopt(g_ListenSock, SOL_SOCKET, SO_SNDBUF,
		(char*)&sockType, &sockTypeLen);
	printf("SendBuffer=%d\n", sockType);
	getsockopt(g_ListenSock, SOL_SOCKET, SO_RCVBUF,
		(char*)&sockType, &sockTypeLen);
	printf("RecvBuffer=%d\n", sockType);

	int optval = 1;
	Check(setsockopt(g_ListenSock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&optval, sizeof(optval)), __LINE__);
	linger optLinger;
	int iLingerLen = sizeof(optLinger);
	optLinger.l_onoff = 1; // 0이면 바로 종료
	optLinger.l_linger = 1000;
	if (setsockopt(g_ListenSock, SOL_SOCKET, SO_LINGER, (char*)&optLinger, iLingerLen) != 0)
	{
		Error("SO_LINGER");
		return false;
	}
	SOCKADDR_IN sa;
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY); // 자신의 모든 IP 값 사용
	sa.sin_port = htons(10000);

	Check(bind(g_ListenSock, (SOCKADDR*)&sa, sizeof(sa)), __LINE__);

	iRet = listen(g_ListenSock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)
	{
		Error("listen");
		return false;
	}
	bool bRecvData = false;
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);

	//넌블로킹 소켓으로 전환
	unsigned long iMode = 1;
	ioctlsocket(g_ListenSock, FIONBIO, &iMode);
	return true;
}
bool DeleteNetwork()
{
	closesocket(g_ListenSock);
	WSACleanup();
	return true;
}
bool AddUser()
{
	int iRet;
	bool bRecvData = false;
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);

	SOCKET client = accept(g_ListenSock, (SOCKADDR*)&clientAddr, &len);
	if (client == INVALID_SOCKET)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}
	else
	{
		sUser user;
		user.sock = client;
		user.addr = clientAddr;
		userlist.push_back(user);
		printf("\n 접속 -> %s : %d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
	}
	return true;
}
void main()
{
	
	if (InitNetwork() == false)
	{

	}
	while (1)
	{
		if (AddUser() == false)
		{
			break;
		}
		if (RecvUserData() == false)
		{
			break;
		}
		if (Broadcastting() == false)
		{
			break;
		}
	}

	DeleteNetwork();
	


}