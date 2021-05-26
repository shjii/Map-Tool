#pragma once
#include "KCoreStd.h"
class KWindow
{
protected:
	HINSTANCE   m_hInstance;
	HWND		m_hWnd;
	DWORD       m_dwExStyle;
	T_STR		m_szClassName;
	T_STR		m_szWindowName;
	DWORD		m_dwStyle;
	bool		m_bFullScreen = false;
public:
	MSG			m_msg;
	RECT		m_rtClient;
	RECT		m_rtWindow;
	bool		m_bDrag;
	POINT		m_ptClick;
public:
	// ��� �Լ�
	bool		SetWindow(HINSTANCE hInstance, const TCHAR* pTitleName = L"K", int iWidth = 800, int iHeight = 600);
	bool		MsgProcess();
public:
	virtual void ResizeDevice(UINT w, UINT h);
	virtual	void MsgEvent(MSG msg);
	virtual LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	KWindow();
	virtual ~KWindow();
};

