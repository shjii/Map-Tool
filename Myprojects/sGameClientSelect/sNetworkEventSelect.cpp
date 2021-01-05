#include "sNetworkClient.h"
bool sNetworkClientEventSelect::InitNetwork(string ip, int iPort)
{
	if (sNetwork::InitNetwork(ip.c_str(), iPort) == false)
	{
		return false;
	}
	return true;
}
bool sNetworkClientEventSelect::InitSocket(string ip, int iPort)
{
	m_EventArray[m_iArrayCount] = WSACreateEvent();
	WSAEventSelect(m_Sock, m_EventArray[m_iArrayCount],
		FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	m_iArrayCount++;
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
bool sNetworkClientEventSelect::Frame()
{
	int iIndex = WSAWaitForMultipleEvents(m_iArrayCount, m_EventArray, FALSE, 0, FALSE);
	if (iIndex == WSA_WAIT_FAILED) return false; // 에러
	if (iIndex == WSA_WAIT_TIMEOUT) return true; // 시간 초과
	iIndex -= WSA_WAIT_EVENT_0; //  - 0

	WSANETWORKEVENTS NetworkEvent;
	for (int iEvent = iIndex; iEvent < m_iArrayCount; iEvent++)
	{
		int iSignal = WSAWaitForMultipleEvents(1, &m_EventArray[iEvent], TRUE, 0, FALSE);
		if (iSignal == WSA_WAIT_FAILED)
		{
			sNetwork::g_bConnect = false;
			return false;
		} // 에러
		if (iSignal == WSA_WAIT_TIMEOUT) continue;

		int iRet = WSAEnumNetworkEvents(m_Sock, m_EventArray[m_iArrayCount], &NetworkEvent);
		if (iRet == SOCKET_ERROR)
		{
			Check(iRet,__LINE__);
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			break;
		}
		if (NetworkEvent.lNetworkEvents & FD_CONNECT)
		{
			if (NetworkEvent.iErrorCode[FD_CONNECT_BIT] != 0)
			{
				sNetwork::g_bConnect = false;
				continue;
			}

			sNetwork::g_bConnect = true;
			SendLoginData(m_Sock, "kgca", "game");

		}

		if (NetworkEvent.lNetworkEvents & FD_CONNECT)
		{
			if (NetworkEvent.iErrorCode[FD_CONNECT_BIT] != 0)
			{
				sNetwork::g_bConnect = false;
				continue;
			}
			if (RecvData(m_User) == false)
			{
				sNetwork::g_bConnect = false;
				continue;
			}
		}

		if (NetworkEvent.lNetworkEvents & FD_WRITE)
		{
			if (NetworkEvent.iErrorCode[FD_WRITE_BIT] != 0)
			{
				sNetwork::g_bConnect = false;
				continue;
			}
		}

		if (NetworkEvent.lNetworkEvents & FD_CLOSE)
		{
			if (NetworkEvent.iErrorCode[FD_CLOSE_BIT] != 0)
			{
				sNetwork::g_bConnect = false;
				continue;
			}
		}

	}
	return true;
}
