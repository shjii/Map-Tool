#pragma once
#include "sThread.h"
class sAcceptor : public sThread
{
public:
	SOCKET m_Sock;
public:
	virtual bool Run()override;
	virtual bool Accept();
	virtual bool AddSession(SOCKET sock, SOCKADDR_IN addr);

	virtual bool InitNetwork(string ip, int iPort);
	virtual bool DeleteNetwork();

	virtual bool InitSocket(string ip, int iPort);
	virtual bool CloseSocket();
public:
	sAcceptor();
	virtual ~sAcceptor();
};

