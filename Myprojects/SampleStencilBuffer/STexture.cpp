#include "STexture.h"

bool  STexture::Init()
{	
	return true;
}
bool  STexture::Load(wstring filename)
{
	////m_hBitmap = LoadDIB(filename.c_str());
	//CreateDIBFromFile(filename);

	//if (m_hBitmap == NULL)
	//{
	//	m_hBitmap = (HBITMAP)LoadImage(g_hInstance,
	//		filename.c_str(),
	//		IMAGE_BITMAP,
	//		0,
	//		0,
	//		LR_DEFAULTSIZE | LR_LOADFROMFILE);
	//	if (m_hBitmap == nullptr)
	//	{
	//		//Error();
	//		return false;
	//	}
	//}
	//m_hMemDC = CreateCompatibleDC(g_hScreenDC);
	//SelectObject(m_hMemDC, m_hBitmap);

	//COLORREF bkColor = RGB(255, 0, 0);
	//HBRUSH m_hbrRed = CreateSolidBrush(bkColor);
	//SelectObject(m_hMemDC, m_hbrRed);

	//GetObject(m_hBitmap, sizeof(BITMAP), &m_BitmapInfo);
	//if (m_BitmapInfo.bmBitsPixel == 32)
	//{
	//	m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	//}
	//else
	//{
	//	m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	//}
	return true;
}
bool  STexture::Frame()
{
	return true;
}
bool  STexture::Release()
{	
	return true;
}
bool  STexture::Render()
{

	return true;
}

STexture::STexture()
{

}

STexture::~STexture()
{
}