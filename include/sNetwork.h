#pragma once
#include "sUser.h"
struct sPacket
{
	sUser* pUser;
	UPACKET packet;
};
class sNetwork
{
public:
	std::list<sUser>		m_UserList;
	SOCKET					m_Sock;
	std::vector<sPacket>	m_recvPacket;
	CRITICAL_SECTION		m_cs;     // userlist
	HANDLE					m_hMutex; // packetlist
	static bool				g_bConnect;
public:
	bool	MakePacket(UPACKET& packet,
		char* msg, int iLen, uint16_t type);
	bool	AddUser(SOCKET sock, SOCKADDR_IN addr);
	bool	DelUser(sUser& user);
	void	AddPacket(sUser& user, UPACKET* packet);
	virtual bool	RecvData(sUser& user);
	virtual bool	SendData(sUser& user, UPACKET& msg);
	int		SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type);
public:
	virtual bool Frame() { return true; };
	virtual bool Run();
	virtual bool RecvUserList();
	virtual bool Broadcastting();
	virtual bool InitNetwork(std::string ip, int iPort);
	virtual bool InitSocket(std::string ip, int iPort);
	virtual bool DeleteNetwork();
	virtual bool Accept();
	virtual bool Process();
	virtual void PacketProcess();
public:
	
public:
	sNetwork();
	virtual ~sNetwork();

};

