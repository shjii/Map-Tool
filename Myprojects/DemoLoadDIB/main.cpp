#include "main.h"
#include "sBitmapManager.h"



bool	main::Init()
{
	m_BGIntro.Init();
	m_BGIntro.Load(L"../../data/bitmap/Loading800x600.bmp");
	m_pDlg = g_BitMapMar.Load(L"../../data/bitmap/Apopup_normal.bmp");
	m_CreateDIB = g_BitMapMar.CreateLoad(
		m_pDlg->m_BitmapInfo.bmWidth,
		m_pDlg->m_BitmapInfo.bmHeight, L"test.bmp");


	return true;
}
bool main::Frame()
{
	for (int i = 0; i < m_CreateDIB->m_BitmapInfo.bmHeight; ++i)
	{
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)m_CreateDIB->m_pRaster)[i * m_CreateDIB->m_BitmapInfo.bmWidth];
		LPBYTE pbDlgRGB = (LPBYTE)&((DWORD*)m_pDlg->m_pRaster)[i * m_pDlg->m_BitmapInfo.bmWidth];
		for (int j = 0; j < m_CreateDIB->m_BitmapInfo.bmWidth; ++j)
		{
			//pbSrcRGB[0] = (cosf(g_fGameTimer)*0.5f + 0.5f) * 255;
			//pbSrcRGB[1] = (sinf(g_fGameTimer)*0.5f + 0.5f) * 255;
			//pbSrcRGB[2] = (cosf(g_fGameTimer)*0.5f + 0.5f) * 255;
			pbSrcRGB[0] = pbDlgRGB[0];
			pbSrcRGB[1] = pbDlgRGB[1];
			pbSrcRGB[2] = pbDlgRGB[2];
			pbSrcRGB[3] = pbDlgRGB[3];

			pbSrcRGB += 4;
			pbDlgRGB += 4;
		}
	}
	return true;
}

bool	main::Render()
{
	m_BGIntro.Render();
	m_CreateDIB->DrawAlphaBlend(400,300);
	m_pDlg->DrawAlphaBlend(0,0);
	return true;
}
bool	main::Release()
{
	m_BGIntro.Release();
	return true;
}

GAMERUN;