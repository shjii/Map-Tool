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
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	int sockType;
	int sockTypeLen = sizeof(int);
	//소켓 타입 확인
	//getsockopt(sock, SOL_SOCKET, SO_TYPE, (char*)&sockType, &sockTypeLen);
	//if (sockType == SOCK_STREAM)
	//{
	//	printf("%s\r\n", "SOCK_STREAM");
	//}
	//else
	//{
	//	printf("%s\r\n", "SOCK_DGRAM");
	//}


	// 송수신 버퍼 크기 변경 부분
	//int iSendBuffer = 100000;
	//setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char *)&iSendBuffer, sockTypeLen);

	//송수신 버퍼 크기 확인
	//getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char *)&sockType, &sockTypeLen);
	//printf("sendBuffer=%d\n" , sockType);
	//getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char *)&sockType, &sockTypeLen);
	//printf("sendBuffer=%d\n", sockType);

	int optval = 1;
	//1개의 프로세스에서만IP & PORT 재사용 가능
	//bind함수에서 오류를 얻는다.
	Check(setsockopt(sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&optval, sizeof(optval)), __LINE__);

	 //다수 프로세스에서 ip & port 재사용 가능
	 //SO_EXCLUSIVEADDRUSE 와 SO_REUSEADDR 동시 사용 불가 // 먼저 사용된 설정만 사용됨.
	//iRet = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
	//if (iRet == SOCKET_ERROR)
	//{
	//	return;
	//}

	linger optLinger;
	int iLingerLen = sizeof(optLinger);
	optLinger.l_onoff = 1; // 0이면 바로 종료
	optLinger.l_linger = 1000;
	if (setsockopt(sock, SOL_SOCKET, SO_LINGER,(char*)&optLinger, iLingerLen) != 0)
	{
		return;
	}

	vector<sMsg> recvlist;

	SOCKADDR_IN sa;
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("192.168.0.155");
	sa.sin_port = htons(10000);
	
	iRet = bind(sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	iRet = listen(sock, SOMAXCONN);
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);
	bool bConnect = false;
	while (1)
	{
		SOCKET Client = accept(sock, (SOCKADDR*)&clientAddr, &len);
		if (Client == INVALID_SOCKET)
		{
			break;
		}
		printf("\n 설정 -> %s : %d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
		int iCount = 0;
		bConnect = true;

		sMsg msg;
		char recvBuf[10000] = { 0, };
		char* pRecv = recvBuf;
		int iRecvSize = 0;
		int iSendSize = 0;
		int iPacketSize = sizeof(sMsg);

		while (bConnect)
		{
			while (iRecvSize < iPacketSize)
			{
				iRecvSize += recv(Client, recvBuf, iPacketSize - iRecvSize, 0);
				if (iRecvSize == 0 || iRecvSize == SOCKET_ERROR)
				{
					bConnect = false;
					return;
				}
			}
			while (iSendSize < iPacketSize && bConnect)
			{
				memcpy(&msg, recvBuf, sizeof(sMsg));
				iSendSize += send(Client, recvBuf, iPacketSize - iSendSize, 0);
				if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
				{
					bConnect = false;
					return;
				}
			}
			iRecvSize = 0;
			iSendSize = 0;
			recvlist.push_back(msg);
			printf("\n%d : %s", msg.iCnt, msg.buffer);
		}
		shutdown(Client, SD_SEND);
		closesocket(Client);
		printf("\n 해제 -> %s : %d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
	}
	closesocket(sock);
	WSACleanup();
	recvlist.clear();
}