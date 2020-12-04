#include "main.h"
#include "sObjectManager.h"

void	main::MsgEvent(MSG msg)
{
	m_Network.MsgEvent(msg);
}
void main::SendTest()
{
	sChatMsg chatmsg;
	memset(&chatmsg, 0, sizeof(chatmsg));
	strcpy_s(chatmsg.szName, "ȫ�浿");
	strcpy_s(chatmsg.buffer, "kgca");
	chatmsg.iCnt = 1;
	UPACKET packet;
	m_Network.MakePacket(packet, (char*)&chatmsg, sizeof(sChatMsg), PACKET_CHAT_MSG);
	m_Network.m_sendPacket.push_back(packet);
}
bool	main::Init()
{
	if (m_Network.InitNetwork("192.168.0.155",10000) == false)
	{
		return false;
	}
	return true;
}
bool	main::Frame()
{
	vector<sPacket>::iterator senditer;
	for (senditer = m_Network.m_recvPackt.begin(); senditer != m_Network.m_recvPackt.end(); senditer++)
	{
		UPACKET* packet = (UPACKET*)&senditer->packet;
		if (packet->ph.type == PACKET_CHAT_MSG)
		{
			static int iCount = 0;
			iCount++;
			sChatMsg* pMsg = (sChatMsg*)&packet->msg;
			sMsg msg;
			msg.msg = to_mw(pMsg->buffer);
			msg.rt = { 100,100, g_rtClient.right, g_rtClient.bottom };
			g_Write.push(msg);
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
				m_Network.SendLoginData(m_Network.m_Sock, "kgca", "game");
			}
		}
	}
	m_Network.m_recvPackt.clear();
	return true;
}
bool	main::Render()
{
	m_Network.SendPackets(m_Network.m_User);

	if (g_KeyMap.bAKey > 0)
	{
		SendTest();
	}
	//sChatMsg chatmsg;
	//memset(&chatmsg, 0 , sizeof(chatmsg));
	//strcpy_s(chatmsg.szName, "ȫ�浿");
	//strcpy_s(chatmsg.buffer, "kgca");
	//chatmsg.iCnt = 1;
	//if (m_Network.SendMsg(m_Network.m_Sock, (char*)&chatmsg, sizeof(sChatMsg), PACKET_CHAT_MSG) < 0)
	//{
	//	return false;
	//}
	return true;
}
bool	main::Release()
{
	m_Network.DeleteNetwork();
	return true;
}
GAMERUN;