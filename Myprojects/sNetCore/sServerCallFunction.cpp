#include "sServer.h"
void sServer::ChatMsg(sPacket& t)
{
	sChatMsg* pMsg = (sChatMsg*)&t.packet.msg;
	printf("\n[%s]%s:%d", pMsg->szName,
		pMsg->buffer, pMsg->iCnt);
	m_SendBroadcastPacketPool.AddPacket(t);
}
void sServer::MoveAction(sPacket& t)
{
	sEnemyPos* pMsg = (sEnemyPos*)&t.packet.msg;
	/*std::cout << pMsg->index << ":" <<	pMsg->p[0] << ":"
		<<pMsg->p[1] << ":" <<	pMsg->p[2] << std::endl;*/
	std::cout << "->" << std::endl;;
	m_SendBroadcastPacketPool.AddPacket(t);
}
void sServer::Login(sPacket& t)
{
	UPACKET sendPacket;
	T_STR szID = L"kgca";
	T_STR szPS = L"game";
	sLogin* login = (sLogin*)t.packet.msg;
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
	MakePacket(sendPacket, (char*)&ret, sizeof(sLoginResult),
		PACKET_LOGIN_ACK);
	if (t.pUser != nullptr)
	{
		//t.pUser->m_SendPacket.push_back(sendPacket);
		sPacket tPacket = t;
		tPacket.pUser = t.pUser;
		tPacket.packet = sendPacket;
		m_SendPacketPool.AddPacket(tPacket);
	}
	std::cout << PACKET_LOGIN_REQ << std::endl;
}