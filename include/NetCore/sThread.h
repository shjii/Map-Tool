#pragma once
#include "sLock.h"
class sThread : public sServerObj
{
public:
	unsigned int m_hThread;
	unsigned int m_iThreadID;
	bool		m_bStarted;
public:
	void		CreateThread();
	static unsigned int WINAPI HandleRunner(LPVOID arg);
	virtual bool Run() { return true; }
public:
	sThread();
	virtual ~sThread();
};

