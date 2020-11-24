#pragma once
#include "sCore.h"
#include "sBitmap.h"
class main : public sCore
{
public:
	sBitmap m_BGIntro;
public:
	bool	Init();
	bool	Render();
	bool	Release();
	bool	Frame();
};

 