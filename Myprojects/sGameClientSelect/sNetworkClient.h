#pragma once
#include "sNetwork.h"
class sNetworkClient : public sNetwork
{
public:
	sUser	m_User;
	std::vector<UPACKET>	m_sendPacket;
public:
	virtual bool RecvData(sUser& user)override;
	virtual void PacketProcess()override;
	virtual bool InitNetwork(string ip, int iPort)override;
	virtual bool InitSocket(string ip, int iPort)override;
public:
	void SendLoginData(SOCKET sock, string id, string ps);
	void MsgEvent(MSG msg);
	bool SendPackets(sUser& user);
	bool SendData(sUser& user, UPACKET &msg);
public:
	sNetworkClient();
	~sNetworkClient();
};

