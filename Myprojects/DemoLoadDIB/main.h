#pragma once
#include "sCore.h"
#include "sBitmap.h"

class main : public sCore
{
public:
	sBitmap m_BGIntro;
	sBitmap* m_pDlg;
	sBitmap* m_CreateDIB;

	BLENDFUNCTION m_BlendFunction;
public:
	bool	Init();
	bool	Render();
	bool	Release();
	bool	Frame();
};

