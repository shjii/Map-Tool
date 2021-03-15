#pragma once
#include "TCore.h"
#include "SFbxObj.h"
class main : public TCore
{
	SFbxObj m_fbxObj;
public:
	bool Init();
};
TGAME_RUN;