#pragma once
#include "sNetwork.h"
class sNetworkThread : public sNetwork
{
public:
	HANDLE m_hAccept;
	HANDLE m_hRead;
	HANDLE m_hSend;
	HANDLE m_hProcess;
public:
	virtual bool InitSocket(string ip, int port)override;
	virtual bool Run();
};

