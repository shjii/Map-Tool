#include "sNetUser.h"
#include "sServer.h"
bool sNetUser::DispatchRead(DWORD dwTrans, OVERLAPPED2* ov)
{
	if (m_iWritePos + dwTrans >= g_iMaxDataBufferSize)
	{
		if (m_iReadPos > 0)
		{
			memmove(m_szDataBuffer, &m_szDataBuffer[m_iPacketPos], m_iReadPos);
		}
		m_iPacketPos = 0;
		m_iWritePos = m_iReadPos;
	}
	// ���� ������ ����
	memcpy(&m_szDataBuffer[m_iWritePos], m_szRecvBuffer, dwTrans);
	m_iWritePos += dwTrans;
	m_iReadPos += dwTrans;

	if (m_iReadPos >= PACKET_HEADER_SIZE)
	{
		UPACKET* packet = (UPACKET*)&m_szDataBuffer[m_iPacketPos];
		// 1�� �̻��� ��Ŷ �����Ͱ� ����
		if (packet->ph.len <= m_iReadPos)
		{
			do {
				sPacket _sPacket;
				_sPacket.pUser = this;
				memcpy(&_sPacket.packet,&m_szDataBuffer[m_iPacketPos], packet->ph.len);
				I_Server.m_RecvPacketPool.AddPacket(_sPacket);

				m_iReadPos -= packet->ph.len;
				m_iPacketPos += packet->ph.len;

				if (m_iReadPos < PACKET_HEADER_SIZE)
				{
					break;
				}
				UPACKET* packet = (UPACKET*)&m_szDataBuffer[m_iPacketPos];
			} while (packet->ph.len <= m_iReadPos);
		}
	}
	WaitReceive();
	return true;
}
bool sNetUser::DispatchWrite(DWORD dwTrans, OVERLAPPED2* ov)
{
	return true;
}
bool sNetUser::WaitReceive()
{
	ZeroMemory(&m_ovRead, sizeof(OVERLAPPED));
	m_ovRead.iType = OVERLAPPED2::MODE_RECV;

	ZeroMemory(m_szRecvBuffer, g_iRecvBufferSize);
	m_wsaRecvBuffer.len = g_iRecvBufferSize;
	m_wsaRecvBuffer.buf = m_szRecvBuffer;
	DWORD cbTrans;
	DWORD dwflags = 0;
	int iRet = WSARecv(m_Sock, &m_wsaRecvBuffer, 1, &cbTrans, &dwflags, (OVERLAPPED*)&m_ovRead, NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}
sNetUser::sNetUser()
{
	m_bExit = false;
	m_iPacketPos = 0; // ��Ŷ�� ���� �ּ�
	m_iWritePos = 0; // ��Ŷ ������ ���� �ּ�
	m_iReadPos = 0; // ��Ŷ ������ ���� �ּ�
}
sNetUser::~sNetUser()
{

}