#pragma once
#include "sCorestd.h"
class sWindow
{
public:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	MSG			m_msg;
public:
	bool		SetWindow(HINSTANCE hInstance);
	bool		MsgProcess();
public:
	sWindow();
	virtual ~sWindow();
};

