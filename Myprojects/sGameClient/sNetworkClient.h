#pragma once
#include "sNetwork.h"
class sNetworkClient : public sNetwork
{
public:
	HANDLE g_hSendThread;
	std::vector<UPACKET>	m_sendPacket;
public:
	bool RecvData(sUser& user)override;
	bool PacketProcess()override;
	bool InitNetwork(string ip, int iPort)override;
	bool InitSocket(string ip, int iPort)override;
public:
	void SendLoginData(SOCKET sock, string id, string ps);
public:
	sNetworkClient();
	~sNetworkClient();
};

