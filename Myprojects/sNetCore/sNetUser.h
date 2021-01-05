#pragma once
#include "sServerObj.h"
#define g_iRecvBufferSize 512
#define g_iMaxDataBufferSize (g_iRecvBufferSize * 3)
class sNetUser;
struct sPacket
{
	sNetUser* pUser;
	UPACKET packet;
};
struct OVERLAPPED2 : OVERLAPPED
{
	enum {MODE_RECV =1, MODE_SEND = 2, MODE_EXIT};
	int iType;
};
class sNetUser
{
public:
	bool	m_bExit;
	SOCKET	m_Sock;
	SOCKADDR_IN	m_SockAddr;
	char	m_szRecvBuffer[g_iRecvBufferSize];
	char	m_szSendBuffer[g_iRecvBufferSize];
	WSABUF	m_wsaRecvBuffer;
	WSABUF	m_wsaSendBuffer;
	T_STR szName;
	OVERLAPPED2 m_ovRead;
	OVERLAPPED2 m_ovSend;
	vector<UPACKET>	m_SendPacket;
public:
	char	m_szDataBuffer[g_iMaxDataBufferSize];
	int		m_iPacketPos = 0;
	int		m_iWritePos = 0;
	int		m_iReadPos = 0;
public:
	virtual bool DispatchRead(DWORD dwTrans, OVERLAPPED2* ov);
	virtual bool DispatchWrite(DWORD dwTrans, OVERLAPPED2* ov);
public:
	virtual bool WaitReceive();
	sNetUser();
	virtual ~sNetUser();
};

