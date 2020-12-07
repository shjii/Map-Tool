#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>
#include <Time.h>
#include <vector>
#include <tchar.h>
#include <conio.h>
#include <algorithm>
#include "sProtocol.h"
#pragma comment (lib, "ws2_32.lib")
typedef std::basic_string<TCHAR>		T_STR;
typedef std::basic_string<CHAR>			C_STR;
using namespace std;
static int g_iChatCount = 0;
bool g_bConnect = false;
HANDLE g_hSendThread;
int SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type);

DWORD WINAPI SendThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;  
	sChatMsg chatmsg;
	while (g_bConnect)
	{
		printf("\nmsg=");
		memset(&chatmsg, 0, sizeof(chatmsg));
		fgets(chatmsg.buffer, 128, stdin);
		strcpy_s(chatmsg.szName, "aaaㄴㅇㄹ");
		chatmsg.buffer[strlen(chatmsg.buffer) - 1] = 0;
		chatmsg.iCnt = g_iChatCount++;
		if (SendMsg(sock, (char*)&chatmsg, sizeof(sChatMsg),PACKET_CHAT_MSG) < 0)
		{
			break;
		}
	}
	return 0;
}
int SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type)
{
	UPACKET packet;
	packet.ph.iotype = 0;
	packet.ph.len = PACKET_HEADER_SIZE + iLen;
	packet.ph.time = time(NULL);
	packet.ph.type = type;
	memcpy(&packet.msg, msg, iLen);

	int iSendSize = 0;
	while (iSendSize < packet.ph.len)
	{
		iSendSize += send(sock, (char*)&packet, packet.ph.len - iSendSize, 0);
		if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
			return 0;
		}
	}
	return iSendSize;
}
void SendLoginData(SOCKET sock, string id, string ps)
{
	sLogin login;
	strcpy_s(login.szID, id.c_str());
	strcpy_s(login.szPS, ps.c_str());
	SendMsg(sock, (char*)&login, sLoginSize, PACKET_LOGIN_REQ);
}

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
	linger optLinger;
	int iLingerLen = sizeof(optLinger);
	optLinger.l_onoff = 1;
	optLinger.l_linger = 1000;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optLinger, iLingerLen) != 0)
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

	SendLoginData(sock, "kgca", "game");

	DWORD dwID;
	g_hSendThread = CreateThread(0,0,SendThread,(LPVOID)sock, 0 , &dwID);

	int iRecvSize = 0;
	char recvBuf[10000] = { 0, };
	int iLen = 0;
	g_bConnect = true;
	while (1)
	{
		iLen = recv(sock, &recvBuf[iRecvSize], PACKET_HEADER_SIZE - iRecvSize, 0);
		if (iLen == 0 || iLen == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				break;
			}
			continue;
		}
		iRecvSize += iLen;
		if (iRecvSize == PACKET_HEADER_SIZE)
		{
			UPACKET* packet = (UPACKET*)&recvBuf;
			while (iRecvSize < packet->ph.len)
			{
				iRecvSize += recv(sock, &recvBuf[iRecvSize], packet->ph.len - iRecvSize, 0);
			}
			if (packet->ph.type == PACKET_CHAT_MSG)
			{
				sChatMsg* pMsg = (sChatMsg*)&packet->msg;
				printf("\nRecv = %d:%s\n", pMsg->iCnt, pMsg->buffer);
			}
			if (packet->ph.type == PACKET_LOGIN_ACK)
			{
				sLoginResult* ret = (sLoginResult*)packet->msg;
				if (ret->iRet == 1)
				{
					ResumeThread(g_hSendThread);
				}
				else
				{
					SendLoginData(sock, "kgca", "game");
				}
			}
			memset(recvBuf, 0, sizeof(char) * 10000);
			iRecvSize = 0;
		}
	}
	g_bConnect = false;
	shutdown(sock, SD_SEND);
	closesocket(sock);
	WSACleanup();
	CloseHandle(g_hSendThread);
}