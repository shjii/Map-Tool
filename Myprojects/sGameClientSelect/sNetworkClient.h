#pragma once
#include "sNetwork.h"
class sNetworkClient : public sNetwork
{
public:
	sUser	m_User;
	std::vector<UPACKET>	m_sendPacket;
	bool					m_bLogin;
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
	virtual ~sNetworkClient();
};

class sNetworkClientSelect : public sNetworkClient
{
public:
	FD_SET m_rSet;
	FD_SET m_wSet;
	FD_SET m_eSet;
public:
	virtual bool Frame()override;
	virtual bool InitNetwork(string ip, int iPort)override;
	virtual bool InitSocket(string ip, int iPort)override;
public:
	sNetworkClientSelect();
	virtual ~sNetworkClientSelect();
};

class sNetworkClientEventSelect : public sNetworkClient
{
public:
	HANDLE		m_EventArray[WSA_MAXIMUM_WAIT_EVENTS];
	int			m_iArrayCount;
public:
	virtual bool Frame()override;
	virtual bool InitNetwork(string ip, int iPort)override;
	virtual bool InitSocket(string ip, int iPort)override;
public:
	sNetworkClientEventSelect() { m_iArrayCount = 0; };
	virtual ~sNetworkClientEventSelect() {};
};
