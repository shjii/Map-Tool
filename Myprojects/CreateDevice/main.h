#pragma once
#include "sCore.h"
#include "SDevice.h"
class main : public sCore
{
public:
	SDevice m_Device;
public:
	bool		Init();
	bool		Frame();
	bool		PreRender();
	bool		Render();
	bool		PostRender();
	bool		Release();
};

