#include "sPacketPool.h"

void sPacketPool::Lock()
{
	WaitForSingleObject(m_hMutexPacketPool, INFINITE);
}
void sPacketPool::Unlock()
{
	ReleaseMutex(m_hMutexPacketPool);
}
void sPacketPool::AddPacket(sPacket & pakcet)
{
	Lock();
	m_list.push_back(pakcet);
	Unlock();
}

bool sPacketPool::MakePacket(UPACKET & packet, char * msg, int iLen, uint16_t type)
{
	packet.ph.iotype = 0;
	packet.ph.len = PACKET_HEADER_SIZE + iLen;
	packet.ph.type = type;
	packet.ph.time = time(NULL);
	memcpy(&packet.msg, msg, iLen);
	return true;
}
void	sPacketPool::Clear()
{
	Lock();
	m_list.clear();
	Unlock();
}
sPacketPool::sPacketPool()
{
	m_hMutexPacketPool = CreateMutex(NULL, FALSE, L"packetpool");
}

sPacketPool::~sPacketPool()
{
	CloseHandle(m_hMutexPacketPool);
}