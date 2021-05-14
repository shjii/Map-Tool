#pragma once
#include "TCore.h"
#include "SHeightMap.h"

class main : public TCore
{
public:
	SHeightMap		m_Map;
	Vector4			m_vDirValue;

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	bool PostRender()override;
};
TGAME_RUN;