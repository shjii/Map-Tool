#pragma once
#include "sStd.h"
class sBitmap
{
public:
	HBITMAP m_hBitmap;
	HDC m_hMemDC;
	BITMAP m_BitmapInfo;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	bool Load(wstring filename);
	bool Draw(RECT desk, RECT rt, DWORD opMode, int a = 0);
	bool DrawColorKey(RECT desk, RECT src, DWORD color = RGB(255, 0, 255));
};

