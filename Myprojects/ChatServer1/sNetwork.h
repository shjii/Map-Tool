#pragma once
#include "sUser.h"

class sNetwork
{
public:
	list<sUser>		 m_Userlist;
	SOCKET			 m_Sock;
	vector<UPACKET>	 m_recvPackt;
public:
	bool MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type);
public:
	bool AddUser(SOCKET sock, SOCKADDR_IN addr);
	bool DelUser(sUser& user);
	void AddPacket(sUser& user, UPACKET* packet);
	bool RecvData(sUser& user);
	bool SendData(sUser& user, UPACKET& msg);
	bool Run();
public:
	bool RecvUserList();
	bool Broadcastting();
	bool InitNetwork();
	bool DeleteNetwork();
	bool Accept();
	bool Process();
public:
	int SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type);
public:
	sNetwork();
	~sNetwork();

};

