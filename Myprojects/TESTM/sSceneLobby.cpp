#include "sSceneLobby.h"
bool sSceneLobby::ReSet()
{
	c = false;
	t = 0.0f;
	a = 0;
	return true;
}
bool	sSceneLobby::Frame()
{
	sScene::Frame();

	if (!c)
	{
		for (auto obj : m_BackobjList)
		{
			if (obj->m_szName != L"BACKGROUND_LOBBY")
			{
				obj->m_rtDesk.left = 1000.0f;
				obj->m_rtDesk.top = 1000.0f;
			}
		}
	}

	t += g_fSecondPerFrame;
	if (t > 0.5f)
	{
		if (g_KeyMap.bAKey >= 1)
		{
			t = 0.0f;
			if (a <= 0)
			{
				a = 3;
			}
			else
			{
				a--;
			}
		}
		if (g_KeyMap.bDKey >= 1)
		{
			t = 0.0f;
			if (a >= 3)
			{
				a = 0;
			}
			else
			{
				a++;
			}
		}
	}


	if (!c)
	{
		switch (a)
		{
		case 0:
		{
			for (auto obj : m_BackobjList)
			{
				if (obj->m_szName == L"P1")
				{
					obj->m_rtDesk.left = P1[a].x;
					obj->m_rtDesk.top = P1[a].y;
				}
				if (obj->m_szName == L"MarcoW")
				{
					obj->m_rtDesk.left = CW[a].x;
					obj->m_rtDesk.top = CW[a].y;
				}
				if (g_KeyMap.bSpace >= 1)
				{
					c = true;
					for (auto objc : m_BackobjList)
					{
						if (obj->m_szName == L"MarcoC")
						{
							obj->m_rtDesk.left = CC[a].x;
							obj->m_rtDesk.top = CC[a].y;
						}
					}
				}
			}
		}break;
		case 1:
		{
			for (auto obj : m_BackobjList)
			{
				if (obj->m_szName == L"P1")
				{
					obj->m_rtDesk.left = P1[a].x;
					obj->m_rtDesk.top = P1[a].y;
				}
				if (obj->m_szName == L"eriW")
				{
					obj->m_rtDesk.left = CW[a].x;
					obj->m_rtDesk.top = CW[a].y;
				}
				if (g_KeyMap.bSpace > 0)
				{
					c = true;
					for (auto objc : m_BackobjList)
					{
						if (obj->m_szName == L"eriC")
						{
							obj->m_rtDesk.left = CC[a].x;
							obj->m_rtDesk.top = CC[a].y;
						}
					}
				}
			}
		}break;
		case 2:
		{
			for (auto obj : m_BackobjList)
			{
				if (obj->m_szName == L"P1")
				{
					obj->m_rtDesk.left = P1[a].x;
					obj->m_rtDesk.top = P1[a].y;
				}
				if (obj->m_szName == L"TarmaW")
				{
					obj->m_rtDesk.left = CW[a].x;
					obj->m_rtDesk.top = CW[a].y;
				}
				if (g_KeyMap.bSpace > 0)
				{
					c = true;
					for (auto objc : m_BackobjList)
					{
						if (obj->m_szName == L"TarmaC")
						{
							obj->m_rtDesk.left = CC[a].x;
							obj->m_rtDesk.top = CC[a].y;
						}
					}
				}
			}
		}break;
		case 3:
		{
			for (auto obj : m_BackobjList)
			{
				if (obj->m_szName == L"P1")
				{
					obj->m_rtDesk.left = P1[a].x;
					obj->m_rtDesk.top = P1[a].y;
				}
				if (obj->m_szName == L"FioW")
				{
					obj->m_rtDesk.left = CW[a].x;
					obj->m_rtDesk.top = CW[a].y;
				}
				if (g_KeyMap.bSpace > 0)
				{
					c = true;
					for (auto objc : m_BackobjList)
					{
						if (obj->m_szName == L"FioC")
						{
							obj->m_rtDesk.left = CC[a].x;
							obj->m_rtDesk.top = CC[a].y;
						}
					}
				}
			}
		}break;
		}
	}
	else
	{
		if (t > 1.0f)
		{
			m_bSceneChange = true;
		}
	}



	return true;
}