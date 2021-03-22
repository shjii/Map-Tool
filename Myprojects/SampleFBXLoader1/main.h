#pragma once
#include "TCore.h"
#include "SFbxObj.h"
class main : public TCore
{
	SFbxObj m_fbxObj;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};
TGAME_RUN;