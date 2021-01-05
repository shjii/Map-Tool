#include "main.h"

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
bool  main::NetInit()
{
	srand(time(NULL));

	m_szName = to_wm(name);
	//m_szName = "ȫ�浿zxc";
	//m_szName += to_string(rand() % 100);
	m_bUseNet = false;
	if (m_Network.InitNetwork(to_wm(IP), 10000) == false)
	{
		return false;
	}
	m_bUseNet = true;
	return true;
}

bool	main::NetFrame()
{
	if (m_bUseNet)
	{
		if (m_Network.Frame() == false)
		{
			m_bUseNet = false;
			return true;
		}
  		vector<sPacket>::iterator senditer;
		for (senditer = m_Network.m_recvPacket.begin(); senditer != m_Network.m_recvPacket.end(); senditer++)
		{
			UPACKET* packet = (UPACKET*)&senditer->packet;
			if (packet->ph.type == PACKET_CHAT_MSG)
			{ 
				static int iCount = 0;
				iCount++;
				sChatMsg* pMsg = (sChatMsg*)&packet->msg;
				sMsg msg;
				msg.msg = L"[";
				msg.msg += to_mw(m_szName);
				msg.msg += L"]: ";
				msg.msg += to_mw(pMsg->buffer);
				int iTop = g_Write.m_TextList.size();
				msg.rt = { 0,iTop * 25,g_rtClient.right, g_rtClient.bottom };
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
		m_Network.m_recvPacket.clear();
		return true;
	}
}
bool	main::NetRender()
{
	if (m_bUseNet && m_Network.m_bLogin)
	{
		if (!m_Network.SendPackets(m_Network.m_User))
		{
			m_bUseNet = false;
			return true;
		}
		fTime += g_fSecondPerFrame;
		if (g_Write.chc(a))
		{
			printf("%s", a.c_str());
			SendTest(to_wm(a).c_str());
		}
	}
}
bool	main::NetRelease()
{
	m_Network.DeleteNetwork();
	return true;
}
//GAMERUN;
