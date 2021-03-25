#pragma once
#include "TCore.h"
#include "SFbxObj.h"
class main : public TCore
{
	vector<shared_ptr<SFbxObj>> ObjList;
	//SFbxObj m_fbxObj;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};
TGAME_RUN;