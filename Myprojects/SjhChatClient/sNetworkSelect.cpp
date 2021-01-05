#include "sNetworkClient.h"

bool sNetworkClientSelect::InitNetwork(string ip, int iPort)
{
	if (sNetwork::InitNetwork(ip.c_str(),iPort) == false)
	{
		return false;
	}
	sNetwork::g_bConnect = true;
	SendLoginData(m_Sock, "kgca", "gmae");
	return true;
}
bool sNetworkClientSelect::InitSocket(string ip, int iPort)
{
	unsigned long iMode = 1;
	ioctlsocket(m_Sock, FIONBIO, &iMode);

	SOCKADDR_IN sa;
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(ip.c_str());
	sa.sin_port = htons(iPort);
	int iRet = connect(m_Sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}
	return true;
}
bool sNetworkClientSelect::Frame()
{
	if (!sNetwork::g_bConnect) return true;

	FD_ZERO(&m_rSet);
	FD_ZERO(&m_wSet);
	FD_ZERO(&m_eSet);
	FD_SET(m_Sock, &m_rSet);
	FD_SET(m_Sock, &m_wSet);
	FD_SET(m_Sock, &m_eSet);

	timeval time;
	time.tv_sec = 5; //초
	time.tv_usec = 0; //마이크로초
	int iRet = select(0, &m_rSet, &m_wSet, &m_eSet,&time);
	if (iRet == 0) return true;
	if (FD_ISSET(m_Sock, &m_eSet))
	{
		sNetwork::g_bConnect = false;
		return true;
	}
	if (FD_ISSET(m_Sock, &m_rSet))
	{
		if (RecvData(m_User) == false)
		{
			sNetwork::g_bConnect = false;
			return true;
		}
	}
	if (FD_ISSET(m_Sock, &m_wSet))
	{
		return true;
	}
	return true;
 }