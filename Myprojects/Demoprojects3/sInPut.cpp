#include "sInPut.h"

TInputActionMap Inputkey;

DWORD sInPut::KeyCheck(DWORD dwKey)
{
	// 동기 GetKeyState
	//비동기
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
	
	Inputkey.bWKey = KeyCheck('W');
	Inputkey.bAKey = KeyCheck('a');
	Inputkey.bSKey = KeyCheck('s');
	Inputkey.bDKey = KeyCheck('d');
	Inputkey.bExit = KeyCheck(VK_ESCAPE);
	Inputkey.bSpace = KeyCheck(VK_SPACE);

	Inputkey.bLeftClick = KeyCheck(VK_LBUTTON);
	Inputkey.bMiddleClick = KeyCheck(VK_MBUTTON);
	Inputkey.bRightClick = KeyCheck(VK_RBUTTON);

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