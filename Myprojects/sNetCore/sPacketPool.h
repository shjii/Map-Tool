#pragma once
#include "sNetUser.h"
class sPacketPool : public sServerObj
{
public:
	HANDLE	m_hMutexPacketPool;
	vector<sPacket>	m_list;
public:
	void	AddPacket(sPacket& packet);
	void	Clear();
	void	Lock();
	void	Unlock();
	bool	MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type);
public:
	sPacketPool();
	~sPacketPool();
};

