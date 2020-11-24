#pragma once
#include "sStd.h"


enum KeyState
{
	KEY_FREE = 0,
	KEY_PUSH,
	KEY_HOLD,
	KEY_UP
};


class sInPut:public GetSing<sInPut>
{
public:

public:
	DWORD m_dwKeyState[256];
	POINT m_MousePos;
public:

	bool	Init();

	bool	Frame();

	bool	Render();
	
	bool	Release();

	DWORD KeyCheck(DWORD dwKey);

private:
	sInPut();
	virtual ~sInPut();
	friend class GetSing<sInPut>;
};
#define g_InPut sInPut::GetInstance()
