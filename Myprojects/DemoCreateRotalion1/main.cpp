#include "main.h"
#include "sBitmapManager.h"

void main::GetRotationBitmap(float fAngle, sBitmap* pSrcBitmap, sBitmap* pDeskBitmap)
{
	assert(pSrcBitmap);
	assert(pDeskBitmap);
	PatBlt(pDeskBitmap->m_hMemDC, 0, 0, pDeskBitmap->m_BitmapInfo.bmWidth, pDeskBitmap->m_BitmapInfo.bmHeight, PATCOPY);
	int iOldGraphic = SetGraphicsMode(pDeskBitmap->m_hMemDC, GM_ADVANCED);
	float fRadian = fAngle * (3.141592f / 180.0f);
	float fCosine = cos(fRadian);
	float Sine = sin(fRadian);
	XFORM xForm;
	xForm.eM11= fCosine; 	xForm.eM12 = Sine;
	xForm.eM21 = -Sine; 	xForm.eM22= fCosine;
	xForm.eDx = pDeskBitmap->m_BitmapInfo.bmWidth/2;
	xForm.eDy = pDeskBitmap->m_BitmapInfo.bmHeight /2;
	SetWorldTransform(pDeskBitmap->m_hMemDC, &xForm);

	BitBlt(pDeskBitmap->m_hMemDC,
		-(pSrcBitmap->m_BitmapInfo.bmWidth / 2),
		-(pSrcBitmap->m_BitmapInfo.bmHeight / 2),
		pSrcBitmap->m_BitmapInfo.bmWidth,
		pSrcBitmap->m_BitmapInfo.bmHeight,
		pSrcBitmap->m_hMemDC, 0, 0, SRCCOPY);

	xForm.eM11 = 1; 	xForm.eM12 = 0;
	xForm.eM21 = 0; 	xForm.eM22 = 1;
	xForm.eDx = 0;   	xForm.eDy = 0 ;
	SetWorldTransform(pDeskBitmap->m_hMemDC, &xForm);
	SetGraphicsMode(pDeskBitmap->m_hMemDC, iOldGraphic);
}

bool	main::Init()
{
	m_BGIntro.Init();
	m_BGIntro.Load(L"../../data/bitmap/Loading800x600.bmp");

	m_pDlg = g_BitMapMar.Load(L"../../data/bitmap/Apopup_normal.bmp");

	//m_pDlggo = g_BitMapMar.Load(L"../../data/bitmap/go.bmp");

	float fDistance = sqrt(m_pDlg->m_BitmapInfo.bmWidth*m_pDlg->m_BitmapInfo.bmWidth +
		m_pDlg->m_BitmapInfo.bmHeight*m_pDlg->m_BitmapInfo.bmHeight);

	m_CreateDIB = g_BitMapMar.CreateLoad(
		fDistance+1,
		fDistance+1,
		L"rotaton.bmp",
		m_pDlg->m_BitmapInfo.bmBitsPixel);

	//fDistance = sqrt(m_pDlggo->m_BitmapInfo.bmWidth*m_pDlggo->m_BitmapInfo.bmWidth +
	//	m_pDlggo->m_BitmapInfo.bmHeight*m_pDlggo->m_BitmapInfo.bmHeight);

	//m_CreateDIBgo = g_BitMapMar.CreateLoad(
	//	fDistance + 1,
	//	fDistance + 1,
	//	L"goq.bmp",
	//	m_pDlggo->m_BitmapInfo.bmBitsPixel);

	return true;
}
bool main::Frame()
{
	GetRotationBitmap(30.0f, m_pDlg, m_CreateDIB);
	//GetRotationBitmap(30, m_pDlggo, m_CreateDIBgo);
	return true;
}

bool	main::Render()
{
	m_BGIntro.Render();

	//m_pDlg->DrawAlphaBlend(0,0);

	/*m_CreateDIB->DrawAlphaBlend(400,300,255);*/
	m_CreateDIB->DrawColorKey(400 - 116, 300 - 116,
		RGB(0, 0, 0));
	//m_CreateDIBgo->DrawColorKey(100, 100, RGB(255,0,255));
	return true;
}
bool	main::Release()
{
	DeleteObject(m_hbrBack);
	DeleteDC(m_hRotationDC);
	m_BGIntro.Release();
	return true;
}

GAMERUN;