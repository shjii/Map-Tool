#include "sBitmap.h"

bool	sBitmap::Init()
{
	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = 255;
	m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
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
bool sBitmap::BackGroundDrow(sPoint m_ptPos)
{
	BitBlt(g_hOffScreenDC, m_ptPos.x, m_ptPos.y, g_rtClient.right, g_rtClient.bottom,
		m_hMemDC, 0, 0, SRCCOPY);
	return true;
}
bool	sBitmap::Release()
{
	DeleteObject(m_hBitmap);
	ReleaseDC(g_hWnd, m_hMemDC);
	return true;
}
HBITMAP sBitmap::LoadDIB(wstring filename)
{
	HANDLE hFile = CreateFile(filename.c_str(),
		GENERIC_READ, 0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	BITMAPFILEHEADER header;
	DWORD dwRead;
	BOOL bRet = ReadFile(hFile, &header, sizeof(BITMAPFILEHEADER), &dwRead, NULL);

	DWORD dwDataSize = header.bfOffBits - sizeof(BITMAPFILEHEADER);

	BITMAPINFO* pBitInfo = (BITMAPINFO*)malloc(dwDataSize);

	bRet = ReadFile(hFile, pBitInfo, dwDataSize, &dwRead, NULL);

	PVOID pRaster = 0;
	m_hBitmap = CreateDIBSection(g_hScreenDC, pBitInfo, DIB_RGB_COLORS, (LPVOID*)(&m_pRaster), NULL, 0);


	ReadFile(hFile, m_pRaster,
		header.bfSize - header.bfOffBits,
		&dwRead, NULL);

	free(pBitInfo);
	CloseHandle(hFile);
	return m_hBitmap;
}
bool sBitmap::Load(wstring filename)
{
	//m_hBitmap = (HBITMAP)LoadImage(g_hInstance,
	//	filename.c_str(), IMAGE_BITMAP, 
	//	0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);

	m_hBitmap = LoadDIB(filename.c_str());
		if (m_hBitmap == NULL)
	{
		return false;
	}
	m_hMemDC = CreateCompatibleDC(g_hScreenDC);
	SelectObject(m_hMemDC, m_hBitmap);

	COLORREF bkColor = RGB(255, 0, 0);
	HBRUSH m_hbrRed = CreateSolidBrush(bkColor);
	SelectObject(m_hMemDC, m_hbrRed);

	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitmapInfo);

	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}
	return true;
}

bool sBitmap::DrawColorKey(RECT desk,
	RECT src, DWORD color)
{
	TransparentBlt(g_hOffScreenDC,
		desk.left, desk.top,
		desk.right, desk.bottom,
		m_hMemDC, src.left, src.top,
		src.right, src.bottom,
		color);

	return true;
}

bool sBitmap::DrawColorKey(LONG x,
	LONG y, DWORD color)
{
	RECT desk = { x,y, m_BitmapInfo.bmWidth,m_BitmapInfo.bmHeight };
	RECT src = { 0,0, m_BitmapInfo.bmWidth,m_BitmapInfo.bmHeight };

	TransparentBlt(g_hOffScreenDC,
		desk.left, desk.top,
		desk.right, desk.bottom,
		m_hMemDC, src.left, src.top,
		src.right, src.bottom,
		color);

	return true;
}

bool sBitmap::Draw(RECT desk, RECT src, DWORD op, int iType)
{
	switch (iType)
	{
	case 1 : 
	{ 		StretchBlt(
		g_hOffScreenDC, (int)desk.left + src.right, (int)desk.top,
		-(src.right), src.bottom,
		m_hMemDC,
		src.left, src.top,
		src.right, src.bottom,
		op); }break;
	case 2:
	{ 		StretchBlt(
		g_hOffScreenDC, (int)desk.left, (int)desk.top + src.bottom,
		src.right, -(src.bottom),
		m_hMemDC,
		src.left, src.top,
		src.right, src.bottom,
		op); }break;
	case 3:
	{		StretchBlt(			
		g_hOffScreenDC, (int)desk.left + src.right,
		(int)desk.top + src.bottom,
		-(src.right), -(src.bottom),
		m_hMemDC,
		src.left, src.top,
		src.right, src.bottom,
		op); }break;
	default:
	{
		StretchBlt(
			g_hOffScreenDC, (int)desk.left, (int)desk.top,
			src.right, src.bottom,
			m_hMemDC,
			src.left, src.top,
			src.right, src.bottom,
			op);
	}
		break;
	}
	return true;
}

bool sBitmap::DrawAlphaBlend(RECT desk,
	RECT src, BYTE alpha, DWORD op, int iType)
{
	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = alpha;
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}

	AlphaBlend(g_hOffScreenDC,
		desk.left, desk.top,
		desk.right, desk.bottom,
		m_hMemDC,
		src.left, src.top,
		src.right, src.bottom,
		m_BlendFunction);

	return true;
}

bool sBitmap::DrawAlphaBlend(RECT desk,
	RECT src, BLENDFUNCTION bf, DWORD op, int iType)
{
	AlphaBlend(g_hOffScreenDC,
		desk.left, desk.top,
		desk.right, desk.bottom,
		m_hMemDC,
		src.left, src.top,
		src.right, src.bottom,
		bf);
	return true;
}

bool sBitmap::DrawAlphaBlend(LONG x,
	LONG y, BYTE alpha, DWORD op, int iType)
{
	RECT desk = {x,y, m_BitmapInfo.bmWidth,m_BitmapInfo.bmHeight};
	RECT src = { 0,0, m_BitmapInfo.bmWidth,m_BitmapInfo.bmHeight };
	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = alpha;
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}

	AlphaBlend(g_hOffScreenDC,
		desk.left, desk.top,
		desk.right, desk.bottom,
		m_hMemDC,
		src.left, src.top,
		src.right, src.bottom,
		m_BlendFunction);

	return true;
}

bool sBitmap::DrawAlphaBlend(LONG x,
	LONG y, BYTE alpha, BLENDFUNCTION bf, int iType)
{
	RECT desk = { x,y, m_BitmapInfo.bmWidth,m_BitmapInfo.bmHeight };
	RECT src = { 0,0, m_BitmapInfo.bmWidth,m_BitmapInfo.bmHeight };
	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = alpha;
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}

	AlphaBlend(g_hOffScreenDC,
		desk.left, desk.top,
		desk.right, desk.bottom,
		m_hMemDC,
		src.left, src.top,
		src.right, src.bottom,
		bf);

	return true;
}

bool sBitmap::CreateDIB(LONG iWidth, LONG iHeight, int iBitCount)
{
	BITMAPINFO bmi;
	memset(&bmi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biBitCount = iBitCount;
	bmi.bmiHeader.biWidth = iWidth;
	bmi.bmiHeader.biHeight = iHeight;
	bmi.bmiHeader.biPlanes = 1;

	m_hBitmap = CreateDIBSection(g_hScreenDC, (BITMAPINFO*)&bmi, DIB_RGB_COLORS, (LPVOID*)&m_pRaster, NULL, 0);

	//for (int i = 0; i < iHeight; i++)
	//{
	//	LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)m_pRaster)[i * iWidth];
	//	for (int j = 0; j < iWidth; j++)
	//	{
	//		pbSrcRGB[0] = 0;
	//		pbSrcRGB[1] = 0;
	//		pbSrcRGB[2] = 0;
	//		pbSrcRGB[3] = 255;
	//		pbSrcRGB += 4;
	//	}
	//}

	m_hMemDC = CreateCompatibleDC(g_hScreenDC);
	SelectObject(m_hMemDC, m_hBitmap);
	COLORREF bkColor = RGB(0, 0, 0);
	HBRUSH m_hbrRed = CreateSolidBrush(bkColor);
	SelectObject(m_hMemDC, m_hbrRed);

	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitmapInfo);
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}
	return true;
}

