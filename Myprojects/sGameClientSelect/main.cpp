#include "main.h"
#include "sObjectManager.h"
static int iCount = 0;
void main::MsgEvent(MSG msg)
{
	m_Network.MsgEvent(msg);
}
void main::SendTest(const char* pData)
{
	sChatMsg chatmsg;
	memset(&chatmsg, 0, sizeof(chatmsg));
	strcpy_s(chatmsg.szName, m_szName.c_str());
	strcpy_s(chatmsg.buffer, pData);
	chatmsg.iCnt = iCount++;
	UPACKET packet;
	m_Network.MakePacket(packet, (char*)&chatmsg, sizeof(sChatMsg), PACKET_CHAT_MSG);
	m_Network.m_sendPacket.push_back(packet);
}
bool	main::Init()
{
	srand(time(NULL));
	m_szName = "ȫ�浿";
	m_szName += to_string(rand() % 100);
	m_bUseNet = false;
	if (m_Network.InitNetwork("192.168.50.82", 10000) == false)
	{
		return false;
	}
	m_bUseNet = true;
	return true;
}
bool	main::Frame()
{
	if (m_bUseNet)
	{
		if (m_Network.Frame() == false)
		{
			m_bUseNet = false;
			return true;
		}
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
				msg.msg += to_wstring(iCount);
				msg.rt = { 100,100, g_rtClient.right, g_rtClient.bottom };
				g_Write.push(msg);
			}
			if (packet->ph.type == PACKET_LOGIN_ACK)
			{
				sLoginResult* ret = (sLoginResult*)packet->msg;
				if (ret->iRet == 1)
				{
					m_Network.m_bLogin = true;
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
}
bool	main::Render()
{
	if (m_bUseNet && m_Network.m_bLogin)
	{
		if(!m_Network.SendPackets(m_Network.m_User))
		{
			m_bUseNet = false;
			return true;
		}
		static float fTime = 0.0f;
		fTime += g_fSecondPerFrame;
		if (fTime >= 0.01f)
		{
			{
				SendTest("left");
			}
			fTime -= 0.01f;
		}
		if (g_KeyMap.bSpace > 0)
		{
			SendTest("right");
		}
		return true;
	}
}
bool	main::Release()
{
	m_Network.DeleteNetwork();
	return true;
}
GAMERUN;