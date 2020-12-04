#pragma once
#include "sNetStd.h"
class sUser
{
public:
	SOCKET m_Sock;
	SOCKADDR_IN addr;
	char recvBuf[10000];
	int iRecvSize;
	int iSendSize;
	T_STR szName;
	vector<UPACKET> m_SendPacket;
	sUser();
	virtual ~sUser();
};

