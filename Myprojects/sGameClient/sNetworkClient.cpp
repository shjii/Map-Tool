#include "sNetworkClient.h"
static int g_iChatCount = 0;
bool g_bConnect = false;

DWORD WINAPI SendThread(LPVOID arg)
{
	sNetwork* net = (sNetwork*)arg;
	sChatMsg chatmsg;
	while (g_bConnect)
	{
		//printf("\nmsg=");
		memset(&chatmsg, 0, sizeof(chatmsg));
		//fgets(chatmsg.buffer, 128, stdin);
		strcpy_s(chatmsg.szName, "ȫ�浿");
		strcpy_s(chatmsg.buffer, "kgca");
		chatmsg.buffer[strlen(chatmsg.buffer) - 1] = 0;
		chatmsg.iCnt = g_iChatCount++;
		EnterCriticalSection(&net->m_cs);
		if (net->SendMsg(net->m_Sock, (char*)&chatmsg,
			sizeof(sChatMsg),
			PACKET_CHAT_MSG) < 0)
		{
			LeaveCriticalSection(&net->m_cs);
			break;
		}
		LeaveCriticalSection(&net->m_cs);
		Sleep(1000);
	}
	return 0;
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
bool sNetworkClient::PacketProcess()
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
				ResumeThread(g_hSendThread);
			}
			else
			{
				SendLoginData(m_Sock, "kgca", "game");
			}
		}
	}
	m_recvPackt.clear();
	return true;
}
bool sNetworkClient::InitNetwork(string ip, int iPort)
{
	if (sNetwork::InitNetwork(ip.c_str(), iPort) == false)
	{
		return false;
	}
	g_bConnect = true;
	SendLoginData(m_Sock, "kgca", "game");
	DWORD dwID;
	g_hSendThread = CreateThread(0,0, SendThread, (LPVOID)this, CREATE_SUSPENDED, &dwID);
	return true;
}
bool sNetworkClient::InitSocket(string ip, int iPort)
{ 
	SOCKADDR_IN sa;
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(ip.c_str());
	sa.sin_port = htons(iPort);
	int iRet = connect(m_Sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return false;
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