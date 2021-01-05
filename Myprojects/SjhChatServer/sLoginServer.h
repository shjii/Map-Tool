#pragma once
#include "sLoginAcceptor.h"
class sLoginServer
{
public:
	sLoginAcceptor m_Acceptor;
	bool m_bRun;
public:
	bool Init();
	bool Run();
	bool Release();
public:
	sLoginServer();
	virtual ~sLoginServer();
};

