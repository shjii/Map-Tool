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
	int iRet;
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	int sockType;
	int sockTypeLen = sizeof(int);
	//���� Ÿ�� Ȯ��
	//getsockopt(sock, SOL_SOCKET, SO_TYPE, (char*)&sockType, &sockTypeLen);
	//if (sockType == SOCK_STREAM)
	//{
	//	printf("%s\r\n", "SOCK_STREAM");
	//}
	//else
	//{
	//	printf("%s\r\n", "SOCK_DGRAM");
	//}


	// �ۼ��� ���� ũ�� ���� �κ�
	//int iSendBuffer = 100000;
	//setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char *)&iSendBuffer, sockTypeLen);

	//�ۼ��� ���� ũ�� Ȯ��
	//getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char *)&sockType, &sockTypeLen);
	//printf("sendBuffer=%d\n" , sockType);
	//getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char *)&sockType, &sockTypeLen);
	//printf("sendBuffer=%d\n", sockType);

	int optval = 1;
	//1���� ���μ���������IP & PORT ���� ����
	//bind�Լ����� ������ ��´�.
	iRet = setsockopt(sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&optval, sizeof(optval));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}

	// �ټ� ���μ������� ip & port ���� ����
	// SO_EXCLUSIVEADDRUSE �� SO_REUSEADDR ���� ��� �Ұ� // ���� ���� ������ ����.
	//iRet = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
	//if (iRet == SOCKET_ERROR)
	//{
	//	return;
	//}

	linger optLinger;
	int iLingerLen = sizeof(optLinger);
	optLinger.l_onoff = 1; // 0�̸� �ٷ� ����
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
		printf("\n ���� -> %s : %d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
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
				iSendSize += recv(Client, recvBuf, iPacketSize - iSendSize, 0);
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
		printf("\n ���� -> %s : %d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
	}
	closesocket(sock);
	WSACleanup();
	recvlist.clear();
}