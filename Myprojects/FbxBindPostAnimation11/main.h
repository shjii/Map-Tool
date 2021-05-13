#pragma once
#include "TCore.h"
#include "SFbxObj.h"
#include "STexture.h"
#define MAX_BONE_MATRICES 255
class main : public TCore
{
	shared_ptr<SFbxObj> m_Obj;
	Matrix ControlMatrix;
	//SFbxObj m_fbxObj;
	int id;
	int idt;
	STexture* NormalMap;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};
TGAME_RUN;