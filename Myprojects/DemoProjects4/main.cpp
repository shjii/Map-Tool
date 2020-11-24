#include "main.h"

bool	main::Init()
{
	s_Write.Init();
	return true;
}
bool	main::Frame()
{	
	if (g_KeyMap.bWKey)
	{
		int k = 0;
	}
	if (g_InPut.KeyCheck(VK_RBUTTON) == KEY_HOLD)
	{
		static int iCnt = 1;
		sMsg msg;
		static float fTimer = 0.0f;
		fTimer += g_fSecondPerFrame;
		if (fTimer > 1.0f)
		{
			msg.msg = to_wstring(iCnt++);
			msg.rt.left = 0;
			msg.rt.top = iCnt * 25;
			if (iCnt > 12) iCnt = 0;
			msg.rt.right = 800;
			msg.rt.bottom = 600;
			s_Write.push(msg);
			fTimer -= 1.0f;
		}
	}
	return true;
}
bool	main::Render()
{
	s_Write.Draw(0, 0, g_Timer.m_szBuffer,0);
	s_Write.Render();
	return true;
}
bool	main::Release()
{
	s_Write.Release();
	return true;
}

GAMERUN;