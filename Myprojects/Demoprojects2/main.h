#pragma once
#include "sCore.h"
#include "sTime.h"
class main : public sCore
{
public:
	sTime sTime;
public:
	bool	Init()		override;
	bool	Frame()		override;
	bool	Render()	override;
	bool	Release()	override;
};