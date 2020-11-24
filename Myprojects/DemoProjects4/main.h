#pragma once
#include "sCore.h"
#include "sWrite.h"
#include "sInPut.h"
class main : public sCore
{
public:
	float m_fNpcAppere;
	sWrite s_Write;
public:
	bool	Init()		override;
	bool	Frame()		override;
	bool	Render()	override;
	bool	Release()	override;
};