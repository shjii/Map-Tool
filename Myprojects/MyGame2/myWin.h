#pragma once
#include "std.h"
class myWin
{
public:
	HINSTANCE m_hInstance;
	HWND m_hwnd;
	MSG msg;
public:
	bool SetWindow(HINSTANCE hInstance);
	bool msgProcess();
public:
	myWin();
	virtual ~myWin();
};

