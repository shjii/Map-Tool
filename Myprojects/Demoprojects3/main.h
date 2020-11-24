#pragma once
#include "sCore.h"

class main : public sCore
{

public:
	bool	Init()		override;
	bool	Frame()		override;
	bool	Render()	override;
	bool	Release()	override;
};