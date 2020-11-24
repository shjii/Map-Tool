#pragma once
#include "sCore.h"
#include "sSoundManager.h"
class main : public sCore
{
public:
	sSound* m_pBG;
	sSound* m_pEffect;
public:
	bool	Init()		override;
	bool	Frame()		override;
	bool	Render()	override;
	bool	Release()	override;
};