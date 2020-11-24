#pragma once
#include "sCore.h"
#include "sBitmap.h"
class main : public sCore
{
public:
	sBitmap m_BGIntro;
	sBitmap* m_pDlg;
	sBitmap* m_pDlggo;
	sBitmap* m_CreateDIB;
	sBitmap* m_CreateDIBgo;
	BLENDFUNCTION m_BlendFunction;

	HBRUSH  m_hbrBack;
	HDC m_hRotationDC;
public:
	bool	Init();
	bool	Render();
	bool	Release();
	bool	Frame();

	void GetRotationBitmap(float fAngle, sBitmap* pSrcBitmap, sBitmap* pDeskBitmap);
};

 