#include "main.h"
#include "sBitmapManager.h"

bool	main::Init()
{
	m_BGIntro.Init();
	m_BGIntro.Load(L"../../data/bitmap/field.bmp");

	return true;
}
bool main::Frame()
{
	return true;
}

bool	main::Render()
{
	static float fScroll = 0.0f;
	fScroll += g_fSecondPerFrame * 200;
	if (fScroll > m_BGIntro.m_BitmapInfo.bmWidth)
	{
		fScroll = 0.0f;
	}
	float left = m_BGIntro.m_BitmapInfo.bmWidth - fScroll;

	BitBlt(g_hOffScreenDC, 0, 0, g_rtClient.right, g_rtClient.bottom,
		m_BGIntro.m_hMemDC, fScroll, 0, SRCCOPY);

	BitBlt(g_hOffScreenDC, left, 0, g_rtClient.right, g_rtClient.bottom,
		m_BGIntro.m_hMemDC, 0, 0, SRCCOPY);
	
	return true;
}
bool	main::Release()
{

	m_BGIntro.Release();
	return true;
}

GAMERUN;