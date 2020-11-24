#include "sInPut.h"
TInputActionMap g_KeyMap;
DWORD sInPut::KeyCheck(DWORD dwKey)
{
	//ºñµ¿±â
	SHORT sKey = GetAsyncKeyState(dwKey);

	if ((sKey & 0x8000) || (sKey & 0x8001))
	{
		if (m_dwKeyState[dwKey] == KEY_FREE || m_dwKeyState[dwKey] == KEY_UP)
		{
			m_dwKeyState[dwKey] = KEY_PUSH;
		}
		else
		{
			m_dwKeyState[dwKey] = KEY_HOLD;
		}
	}
	else
	{
		if (m_dwKeyState[dwKey] == KEY_HOLD || m_dwKeyState[dwKey] == KEY_PUSH)
		{
			m_dwKeyState[dwKey] = KEY_UP;
		}
		else
		{
			m_dwKeyState[dwKey] = KEY_FREE;
		}
	}

	return m_dwKeyState[dwKey];
}

bool sInPut::Init()
{
	ZeroMemory(m_dwKeyState, sizeof(DWORD) * 256);
	return true;
}

bool sInPut::Frame()
{
	GetCursorPos(&m_MousePos);
	ScreenToClient(g_hWnd, &m_MousePos);
	
	g_KeyMap.bWKey = max(KeyCheck('W'), KeyCheck(VK_UP));
	g_KeyMap.bAKey = max(KeyCheck('A'), KeyCheck(VK_LEFT));
	g_KeyMap.bSKey = max(KeyCheck('S'), KeyCheck(VK_DOWN));
	g_KeyMap.bDKey = max(KeyCheck('D'), KeyCheck(VK_RIGHT));
	g_KeyMap.bZKey = KeyCheck('Z');
	g_KeyMap.bXKey = KeyCheck('X');
	g_KeyMap.bExit = KeyCheck(VK_ESCAPE);
	g_KeyMap.bSpace = KeyCheck(VK_SPACE);

	g_KeyMap.bLeftClick = KeyCheck(VK_LBUTTON);
	g_KeyMap.bMiddleClick = KeyCheck(VK_MBUTTON);
	g_KeyMap.bRightClick = KeyCheck(VK_RBUTTON);

	return true;
}

	

bool sInPut::Render()
{


	return true;
}

bool sInPut::Release()
{
	return true;
}


sInPut::sInPut()
{

}
sInPut::~sInPut()
{

}