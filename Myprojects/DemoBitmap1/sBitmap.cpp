#include "sBitmap.h"

bool	sBitmap::Init()
{
	return true;
}
bool	sBitmap::Frame()
{
	return true;
}
bool	sBitmap::Render()
{
	BitBlt(g_hOffScreenDC, 0, 0, g_rtClient.right, g_rtClient.bottom,
		m_hMemDC, 0, 0, SRCCOPY);
	return true;
}
bool	sBitmap::Release()
{
	DeleteObject(m_hBitmap);
	ReleaseDC(g_hWnd, m_hMemDC);
	return true;
}
bool sBitmap::Load(wstring filename)
{
	m_hBitmap = (HBITMAP)LoadImage(g_hInstance, filename.c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);

	if (m_hBitmap == NULL)
	{
		return false;
	}

	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitmapInfo);

	m_hMemDC = CreateCompatibleDC(g_hScreenDC);

	SelectObject(m_hMemDC, m_hBitmap);

	COLORREF bkColor = RGB(255, 0, 0);
	HBRUSH m_hbrRed = CreateSolidBrush(bkColor);
	SelectObject(m_hMemDC, m_hbrRed);

	return true;
}

bool sBitmap::DrawColorKey(RECT desk, RECT src, DWORD color )
{
	TransparentBlt(g_hOffScreenDC,
		desk.left, desk.top,
		desk.right, desk.bottom,
		m_hMemDC, src.left, src.top,
		src.right, src.bottom,
		color);

	return true;
}

bool sBitmap::Draw(RECT desk, RECT rt, DWORD opMode, int a)
{
	switch (a)
	{
	case 1 : 
	{ 		StretchBlt(
		g_hOffScreenDC, (int)desk.left + rt.right, (int)desk.top,
		-(rt.right), rt.bottom,
		m_hMemDC,
		rt.left, rt.top,
		rt.right, rt.bottom,
		opMode); }break;
	case 2:
	{ 		StretchBlt(
		g_hOffScreenDC, (int)desk.left, (int)desk.top + rt.bottom,
		rt.right, -(rt.bottom),
		m_hMemDC,
		rt.left, rt.top,
		rt.right, rt.bottom,
		opMode); }break;
	case 3:
	{		StretchBlt(			
		g_hOffScreenDC, (int)desk.left + rt.right,
		(int)desk.top + rt.bottom,
		-(rt.right), -(rt.bottom),
		m_hMemDC,
		rt.left, rt.top,
		rt.right, rt.bottom,
		opMode); }break;
	default:
	{
		StretchBlt(
			g_hOffScreenDC, (int)desk.left, (int)desk.top,
			rt.right, rt.bottom,
			m_hMemDC,
			rt.left, rt.top,
			rt.right, rt.bottom,
			opMode);
	}
		break;
	}
	return true;
}