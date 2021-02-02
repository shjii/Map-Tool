#pragma once
#include "sCorestd.h"

class sWindow
{
public:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	MSG			m_msg;
	RECT		m_rtClient;
public:
	bool		SetWindow(HINSTANCE hInstance);
	bool		MsgProcess();
public:
	virtual		void MsgEvent(MSG msg);
public:
	sWindow();
	virtual ~sWindow();
};

