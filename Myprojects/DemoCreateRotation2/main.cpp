#include "main.h"
#include "sBitmapManager.h"

void main::GetRotationBitmap(float fAngle, int w, int h)
{
	PatBlt(m_CreateDIB->m_hMemDC, 0, 0, w, h, PATCOPY);
	int iOldGraphic = SetGraphicsMode(m_CreateDIB->m_hMemDC, GM_ADVANCED);
	float fRadian = fAngle * (3.141592f / 180.0f);
	float fCosine = cos(fRadian);
	float Sine = sin(fRadian);
	XFORM xForm;
	xForm.eM11= fCosine; 	xForm.eM12 = Sine;
	xForm.eM21 = -Sine; 	xForm.eM22= fCosine;
	xForm.eDx =w/2;   	xForm.eDy = h/2;
	SetWorldTransform(m_CreateDIB->m_hMemDC, &xForm);

	BitBlt(m_CreateDIB->m_hMemDC,
		-(m_pDlg->m_BitmapInfo.bmWidth / 2),
		-(m_pDlg->m_BitmapInfo.bmHeight / 2),
		m_pDlg->m_BitmapInfo.bmWidth,
		m_pDlg->m_BitmapInfo.bmHeight,
		m_pDlg->m_hMemDC, 0, 0, SRCCOPY);

	xForm.eM11 = 1; 	xForm.eM12 = 0;
	xForm.eM21 = 0; 	xForm.eM22 = 1;
	xForm.eDx = 0;   	xForm.eDy = 0 ;
	SetWorldTransform(m_CreateDIB->m_hMemDC, &xForm);
	SetGraphicsMode(m_CreateDIB->m_hMemDC, iOldGraphic);
}

bool	main::Init()
{
	m_BGIntro.Init();
	m_BGIntro.Load(L"../../data/bitmap/Loading800x600.bmp");

	m_pDlg = g_BitMapMar.Load(L"../../data/bitmap/Apopup_normal.bmp");

	float fDistance = sqrt(m_pDlg->m_BitmapInfo.bmWidth*m_pDlg->m_BitmapInfo.bmWidth +
		m_pDlg->m_BitmapInfo.bmHeight*m_pDlg->m_BitmapInfo.bmHeight);

	m_CreateDIB = g_BitMapMar.CreateLoad(
		fDistance+1,
		fDistance+1,
		L"rotaton.bmp");

	return true;
}
bool main::Frame()
{
	GetRotationBitmap(30,
		m_CreateDIB->m_BitmapInfo.bmWidth,
		m_CreateDIB->m_BitmapInfo.bmWidth);
	return true;
}

bool	main::Render()
{
	m_BGIntro.Render();

	//m_pDlg->DrawAlphaBlend(0,0);

	m_CreateDIB->DrawAlphaBlend(400,300,255);
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