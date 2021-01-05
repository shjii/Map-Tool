#include "sNetworkClient.h"
#include "sStd.h"
#define NETWORK_MSG WM_USER+80
static int	g_iChatCount = 0;

void sNetworkClient::MsgEvent(MSG msg)
{
	switch (msg.message)
	{
	case NETWORK_MSG:
	{
		if (WSAGETSELECTERROR(msg.lParam) != 0)
		{
			sNetwork::g_bConnect = false;
			return;
		}
		WORD dwSelect = WSAGETSELECTERROR(msg.lParam);
		switch (dwSelect)
		{
		case FD_CONNECT:
		{
			sNetwork::g_bConnect = true;
			SendLoginData(m_Sock, "kgca", "game");
		}break;
		case FD_CLOSE:
		{
			sNetwork::g_bConnect = false;
		}break;
		case FD_READ:
		{
			RecvData(m_User);
		}break;
		case FD_WRITE:
		{

		}break;
		}
	}break;
	}
}
bool sNetworkClient::SendPackets(sUser& user)
{
	vector<UPACKET>::iterator senditer;
	for (senditer = m_sendPacket.begin(); senditer != m_sendPacket.end(); senditer++)
	{
		if (SendData(user, *senditer) == false)
		{
			sNetwork::g_bConnect = false;
			return false;
		}
	}
	m_sendPacket.clear();
	return true;
}
bool sNetworkClient::SendData(sUser& user, UPACKET &msg)
{
	while (user.iSendSize < msg.ph.len)
	{
		user.iSendSize += send(m_Sock, (char*)&msg, msg.ph.len - user.iSendSize, 0);
		if (user.iSendSize == 0)
		{
			return false;
		}
		if (user.iSendSize == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			user.iSendSize = 0;
			break;
		}
	}
	if (user.iSendSize == msg.ph.len)
	{
		user.iSendSize = 0;
	}
	return true;
}

bool sNetworkClient::RecvData(sUser& user)
{
	if (user.iRecvSize < PACKET_HEADER_SIZE)
	{
		user.iRecvSize += recv(m_Sock, &user.recvBuf[user.iRecvSize], PACKET_HEADER_SIZE - user.iRecvSize, 0);
		if (user.iRecvSize == 0 || user.iRecvSize == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			return true;
		}
	}
	else
	{
		UPACKET* packet = (UPACKET*)user.recvBuf;
		user.iRecvSize += recv(m_Sock, &user.recvBuf[user.iRecvSize], packet->ph.len - user.iRecvSize, 0);
		if (user.iRecvSize == 0 || user.iRecvSize == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			return true;
		}
		if (user.iRecvSize == packet->ph.len)
		{
			AddPacket(user, packet);
			memset(user.recvBuf, 0, sizeof(char) * 10000);
			user.iRecvSize = 0;
		}
	}
	return true;
}
void sNetworkClient::PacketProcess()
{
	vector<sPacket>::iterator senditer;
	for (senditer = m_recvPackt.begin(); senditer != m_recvPackt.end(); senditer++)
	{
		UPACKET* packet = (UPACKET*)&senditer->packet;
		if (packet->ph.type == PACKET_CHAT_MSG)
		{
			sChatMsg* pMsg = (sChatMsg*)&packet->msg;
			printf("\n[%s]%s:%d", pMsg->szName, pMsg->buffer, pMsg->iCnt);
		}
		if (packet->ph.type == PACKET_LOGIN_ACK)
		{
			sLoginResult* ret = (sLoginResult*)packet->msg;
			if (ret->iRet == 1)
			{
				//ResumeThread(g_hSendThread);
			}
			else
			{
				SendLoginData(m_Sock, "kgca", "game");
			}
		}
	}
	m_recvPackt.clear();
}
bool sNetworkClient::InitNetwork(string ip, int iPort)
{
	if (sNetwork::InitNetwork(ip.c_str(), iPort) == false)
	{
		return false;
	}
	return true;
}
bool sNetworkClient::InitSocket(string ip, int iPort)
{ 
	// 넌블로킹소켓 : 네트워크 이벤트
	int iRet = WSAAsyncSelect(m_Sock,
		g_hWnd, NETWORK_MSG,
		FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	if (iRet == SOCKET_ERROR) return false;

	SOCKADDR_IN sa;
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(ip.c_str());
	sa.sin_port = htons(iPort);
	iRet = connect(m_Sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}
	return true;
}
void sNetworkClient::SendLoginData(SOCKET sock, string id, string ps)
{
	sLogin login;
	strcpy_s(login.szID, id.c_str());
	strcpy_s(login.szPS, ps.c_str());
	SendMsg(sock, (char*)&login, sLoginSize, PACKET_LOGIN_REQ);
}
sNetworkClient::sNetworkClient()
{

}
sNetworkClient::~sNetworkClient()
{

}