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
	list<sUser>		 m_Userlist;
	SOCKET			 m_Sock;
	vector<sPacket>	 m_recvPackt;
	CRITICAL_SECTION		m_cs;
	static bool g_bConnect;
public:
	bool MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type);
	bool AddUser(SOCKET sock, SOCKADDR_IN addr);
	bool DelUser(sUser& user);
	void AddPacket(sUser& user, UPACKET* packet);
	virtual bool RecvData(sUser& user);
	virtual bool SendData(sUser& user, UPACKET& msg);
	int SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type);
public:
	virtual bool Run();
	virtual void PacketProcess();
	virtual bool RecvUserList();
	virtual bool Broadcastting();
	virtual bool InitNetwork(string ip, int iPort);
	virtual bool InitSocket(string ip, int iPort);
	virtual bool DeleteNetwork();
	virtual bool Accept();
	virtual bool Process();
public:
	
public:
	sNetwork();
	virtual ~sNetwork();

};

