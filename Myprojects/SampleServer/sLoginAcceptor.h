#pragma once
#include "sAcceptor.h"
#include "sChatUser.h"
class sLoginAcceptor : public sAcceptor
{
public:
	virtual bool AddSession(SOCKET sock, SOCKADDR_IN addr)override;
};

