#include "main.h"
#include "sObjectManager.h"

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
	EnterCriticalSection(&m_Network.m_cs);
	m_Network.RecvData(m_User);
	LeaveCriticalSection(&m_Network.m_cs);
	return true;
}
bool	main::Render()
{
	m_Network.PacketProcess();

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