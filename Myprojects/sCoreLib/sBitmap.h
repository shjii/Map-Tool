#pragma once
#include "sStd.h"
class sBitmap
{
public:
	HBITMAP m_hBitmap;
	HDC m_hMemDC;
	LPBYTE	m_pRaster;
	BITMAP m_BitmapInfo;
	BLENDFUNCTION m_BlendFunction;
public:
	LPVOID m_pBits;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	bool Load(wstring filename);
	bool  Draw(RECT desk, RECT src, DWORD op = SRCCOPY, int iType = 0);
	bool  DrawColorKey(RECT desk,RECT src, DWORD color = RGB(255, 0, 255));
	bool DrawColorKey(LONG x,LONG y, DWORD color);
	HBITMAP LoadDIB(wstring filename);
	bool DrawAlphaBlend(RECT desk,
		RECT src, BYTE alpha = 255, DWORD op = SRCCOPY, int iType = 0);
	bool DrawAlphaBlend(RECT desk,
		RECT src, BLENDFUNCTION bf, DWORD op = SRCCOPY, int iType = 0);
	bool DrawAlphaBlend(LONG x,
		LONG y, BYTE alpha = 255, DWORD op = SRCCOPY, int iType = 0);
	bool CreateDIB(LONG iWidth, LONG iHeight, int iBitCount);
	bool DrawAlphaBlend(LONG x,
		LONG y, BYTE alpha, BLENDFUNCTION bf, int iType = 0);
	bool BackGroundDrow(sPoint m_ptPos);
};

