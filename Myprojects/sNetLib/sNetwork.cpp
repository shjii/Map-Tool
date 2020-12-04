#include "sNetwork.h"
bool sNetwork::MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type)
{
	packet.ph.iotype = 0;
	packet.ph.len = PACKET_HEADER_SIZE + iLen;
	packet.ph.type = type;
	packet.ph.time = time(NULL);
	memcpy(&packet.msg,msg,iLen);
	return true;
}
void sNetwork::PacketProcess()
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
		if (packet->ph.type == PACKET_LOGIN_REQ)
		{
			UPACKET sendPacket;
			T_STR szID = L"kgca";
			T_STR szPS = L"game";
			sLogin* login = (sLogin*)packet->msg;
			T_STR szIDUser = to_mw(login->szID);
			T_STR szPSUser = to_mw(login->szPS);
			sLoginResult ret;
			if (szID == szIDUser && szPS == szPSUser)
			{
				ret.iRet = 1;
			}
			else
			{
				ret.iRet = 0;
			}
			MakePacket(sendPacket, (char*)&ret, sizeof(sLoginResult), PACKET_LOGIN_ACK);
			senditer->pUser->m_SendPacket.push_back(sendPacket);
		}
	}
}
bool sNetwork::Process()
{
	PacketProcess();

	list<sUser>::iterator iter;
	for (iter = m_Userlist.begin(); iter != m_Userlist.end();)
	{
		bool bDelete = false;
		vector<UPACKET>::iterator senditer;
		for (senditer = iter->m_SendPacket.begin();
			senditer != iter->m_SendPacket.end();
			senditer++)
		{
			if (SendData(*iter, *senditer) == false)
			{
				bDelete = true;
				break;
			}
		}
		iter->m_SendPacket.clear();

		if (bDelete == true)
		{
			DelUser(*iter);
			iter = m_Userlist.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	return Broadcastting();
}
int sNetwork::SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type)
{
	UPACKET packet;
	MakePacket(packet ,msg, iLen, type);

	int iSendSize = 0;
	int iRet = 0;
	while (iSendSize < packet.ph.len)
	{
		iRet = send(sock, (char*)&packet,
			packet.ph.len - iSendSize, 0);
		if (iRet == 0 || iRet == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
			return 0;
		}
		iSendSize += iRet;
	}
	return iSendSize;
}
bool sNetwork::Run()
{
	while (1)
	{
		if (Accept() == false)
		{
			break;
		}
		if (RecvUserList() == false)
		{
			break;
		}
		if (Process() == false)
		{
			break;
		}
	}
	return true;
}
bool sNetwork::AddUser(SOCKET sock, SOCKADDR_IN addr)
{
	sUser user;
	user.m_Sock = sock;
	user.addr = addr;
	m_Userlist.push_back(user);
	printf("\n 접속 -> %s : %d", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	return true;
}
bool sNetwork::DelUser(sUser& user)
{
	shutdown(user.m_Sock, SD_SEND);
	closesocket(user.m_Sock);
	printf("\n 해제 -> %s : %d", inet_ntoa(user.addr.sin_addr), ntohs(user.addr.sin_port));
	return true;
}
void sNetwork::AddPacket(sUser& user, UPACKET* packet)
{
	sPacket SPacket;
	SPacket.packet = *packet;
	SPacket.pUser = &user;
	m_recvPackt.push_back(SPacket);
	user.iRecvSize = 0;
}
bool sNetwork::RecvUserList()
{
	list<sUser>::iterator iter;
	for (iter = m_Userlist.begin(); iter != m_Userlist.end();)
	{
		if (RecvData(*iter) == false)
		{
			DelUser(*iter);
			iter = m_Userlist.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	return true;
}
bool sNetwork::RecvData(sUser& user)
{
	int iLen = recv(user.m_Sock, &user.recvBuf[user.iRecvSize], PACKET_HEADER_SIZE - user.iRecvSize, 0);
	if (iLen == 0 || iLen == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
		return true;
	}
	user.iRecvSize += iLen;
	if (user.iRecvSize == PACKET_HEADER_SIZE)
	{
		UPACKET* packet = (UPACKET*)&user.recvBuf;
		while (user.iRecvSize < packet->ph.len)
		{
			user.iRecvSize += recv(user.m_Sock,&user.recvBuf[user.iRecvSize], packet->ph.len - user.iRecvSize, 0);
		}

		AddPacket(user, packet);
		memset(user.recvBuf, 0, sizeof(char) * 10000);
		user.iRecvSize = 0;
	}
	return true;
}
bool sNetwork::SendData(sUser& user, UPACKET& msg)
{
	while (user.iSendSize < msg.ph.len)
	{
		user.iSendSize += send(user.m_Sock, (char *)&msg, msg.ph.len - user.iSendSize, 0);
		if (user.iSendSize == 0 )
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
bool sNetwork::Broadcastting()
{
	list<sUser>::iterator iter;
	for (iter = m_Userlist.begin(); iter != m_Userlist.end();)
	{
		bool bDelete = false;
		vector<sPacket>::iterator senditer;
		for (senditer = m_recvPackt.begin();
			senditer != m_recvPackt.end();
			senditer++)
		{
			if (SendData(*iter, senditer->packet) == false)
			{
				bDelete = true;
				break;
			}
		}
		if (bDelete == true)
		{
			DelUser(*iter);
			iter = m_Userlist.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	m_recvPackt.clear();
	return true;
}
bool sNetwork::InitSocket(string ip, int iPort)
{
	SOCKADDR_IN sa;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY); // 자신의 모든 IP 값 사용
	sa.sin_port = htons(iPort);

	Check(bind(m_Sock, (SOCKADDR*)&sa, sizeof(sa)), __LINE__);

	int iRet = listen(m_Sock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)
	{
		Error("listen");
		return false;
	}
}
bool sNetwork::InitNetwork(string ip, int iPort)
{
	InitializeCriticalSection(&m_cs);
	WSADATA wsa;
	int iRet;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Sock == INVALID_SOCKET)
	{
		Error("socket");
		return false;
	}
	int sockType;
	int sockTypeLen = sizeof(int);

	int iSendBuffer = 100000;
	setsockopt(m_Sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&iSendBuffer, sockTypeLen);
	getsockopt(m_Sock, SOL_SOCKET, SO_TYPE,
		(char*)&sockType, &sockTypeLen);
	if (sockType == SOCK_STREAM)
		printf("%s\r\n", "SOCK_STREAM.");
	else
		printf("%s\r\n", "SOCK_DGRAM");

	getsockopt(m_Sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&sockType, &sockTypeLen);
	printf("SendBuffer=%d\n", sockType);
	getsockopt(m_Sock, SOL_SOCKET, SO_RCVBUF,
		(char*)&sockType, &sockTypeLen);
	printf("RecvBuffer=%d\n", sockType);

	int optval = 1;
	Check(setsockopt(m_Sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&optval, sizeof(optval)), __LINE__);
	linger optLinger;
	int iLingerLen = sizeof(optLinger);
	optLinger.l_onoff = 1; // 0이면 바로 종료
	optLinger.l_linger = 1000;
	if (setsockopt(m_Sock, SOL_SOCKET, SO_LINGER, (char*)&optLinger, iLingerLen) != 0)
	{
		Error("SO_LINGER");
		return false;
	}
	
	if (InitSocket(ip, iPort) == false)
	{
		return false;
	}
	//넌블로킹 소켓으로 전환
	unsigned long iMode = 1;
	ioctlsocket(m_Sock, FIONBIO, &iMode);

	return true;
}
bool sNetwork::DeleteNetwork()
{
	closesocket(m_Sock);
	WSACleanup();
	DeleteCriticalSection(&m_cs);
	return true;
}
bool sNetwork::Accept()
{
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);

	SOCKET client = accept(m_Sock, (SOCKADDR*)&clientAddr, &len);
	if (client == INVALID_SOCKET)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}
	else
	{
		AddUser(client, clientAddr);
	}
	return true;
}

sNetwork::sNetwork() {}
sNetwork::~sNetwork() {}