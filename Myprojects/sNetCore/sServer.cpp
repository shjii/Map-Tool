#include "sServer.h"

bool sServer::MakePacket(UPACKET& packet,
	char* msg, int iLen, uint16_t type)
{
	packet.ph.iotype = 0;
	packet.ph.len = PACKET_HEADER_SIZE + iLen;
	packet.ph.type = type;
	packet.ph.time = time(NULL);
	memcpy(&packet.msg, msg, iLen);
	return true;
}
bool sServer::SendData(sNetUser & user, UPACKET & msg)
{
	DWORD dwSendByte;
	int iRet;
	user.m_wsaSendBuffer.buf = (char*)&msg;
	user.m_wsaSendBuffer.len = msg.ph.len;
	ZeroMemory(&user.m_ovSend, sizeof(OVERLAPPED));
	user.m_ovSend.iType = OVERLAPPED2::MODE_SEND;

	iRet = WSASend(user.m_Sock, &user.m_wsaSendBuffer, 1,
		&dwSendByte, 0, (LPOVERLAPPED)&user.m_ovSend, NULL);

	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}

bool sServer::Broadcastting()
{
	m_SendBroadcastPacketPool.Lock();
	std::map<SOCKET, sNetUser*>::iterator iter;
	for (iter = I_Session.m_UserList.begin();
		iter != I_Session.m_UserList.end();
		)
	{
		sNetUser* pUser = iter->second;
		bool bDelete = false;
		std::vector<sPacket>::iterator senditer;
		for (senditer = m_SendBroadcastPacketPool.m_list.begin();
			senditer != m_SendBroadcastPacketPool.m_list.end();
			senditer++)
		{
			if (SendData(*pUser, senditer->packet) == false)
			{
				bDelete = true;
				break;
			}
		}
		if (bDelete == true)
		{
			I_Session.CloseUser(pUser);
			iter = I_Session.m_UserList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	m_SendBroadcastPacketPool.Unlock();
	m_SendBroadcastPacketPool.Clear();
	return true;
}

bool sServer::Run()
{
	while (m_bStarted)
	{
#pragma region m_RecvPacketPool
		m_RecvPacketPool.Lock();
		std::vector<sPacket>::iterator senditer;
		for (senditer = m_RecvPacketPool.m_list.begin();
			senditer != m_RecvPacketPool.m_list.end();
			senditer++)
		{
			UPACKET* packet = (UPACKET*)&senditer->packet;
			FunctionIterator calliter = m_fnExecutePacket.find(packet->ph.type);
			if (calliter != m_fnExecutePacket.end())
			{
				CallFuction call = calliter->second;
				(this->*call)(*senditer);
			}
		}
		m_RecvPacketPool.m_list.clear();
		m_RecvPacketPool.Unlock();

#pragma endregion m_RecvPacketPool
#pragma region m_SendPacketPool
		{
			m_SendPacketPool.Lock();
			std::vector<sPacket>::iterator senditer;
			for (senditer = m_SendPacketPool.m_list.begin();
				senditer != m_SendPacketPool.m_list.end();
				senditer++)
			{
				sNetUser* pUser = senditer->pUser;
				if (SendData(*senditer->pUser, senditer->packet) == false)
				{
					pUser->m_bExit = true;
					I_Session.DelUser(pUser);
				}
			}
			m_SendPacketPool.Unlock();
			m_SendPacketPool.Clear();
			/*TLock lock((TServerObj*)&I_Session);
			std::map<SOCKET, TNetUser*>::iterator iter;
			for (iter = I_Session.m_UserList.begin();
				iter != I_Session.m_UserList.end();
				)
			{
				TNetUser* pUser = iter->second;
				bool bDelete = false;
				std::vector<UPACKET>::iterator senditer;
				for (senditer = pUser->m_SendPacket.begin();
					senditer != pUser->m_SendPacket.end();
					senditer++)
				{
					if (SendData(*pUser, *senditer) == false)
					{
						pUser->m_bExit = true;
						bDelete = true;
						break;
					}
				}
				pUser->m_SendPacket.clear();

				if (bDelete == true)
				{
					I_Session.CloseUser(pUser);
					iter = I_Session.m_UserList.erase(iter);
				}
				else
				{
					iter++;
				}
			}
			*/
		}
#pragma endregion m_SendPacketPool
#pragma region m_SendBroadcastPacketPool
		{
			sLock lock((sServerObj*)&I_Session);
			Broadcastting();
		}
#pragma endregion m_SendBroadcastPacketPool
	}
	return false;
}

bool sServer::Init()
{
	m_fnExecutePacket[PACKET_CHAT_MSG] = &sServer::ChatMsg;
	m_fnExecutePacket[PACKET_USER_POSITION] = &sServer::MoveAction;
	m_fnExecutePacket[PACKET_LOGIN_REQ] = &sServer::Login;
	return false;
}

sServer::sServer()
{
	CreateThread();
}

sServer::~sServer()
{
}
