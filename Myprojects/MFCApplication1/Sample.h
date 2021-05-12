#pragma once
#include "TCore.h"
#include "SQuadTree.h"
#include "SModel.h"
#include "sCollision.h"
#include "SMousePicking.h"
#include "SMiniMap.h"
#include "SText.h"
class Sample : public TCore
{
public:
	ComPtr<ID3D11Buffer>			m_ConstantBuffer;
	SModel			m_pObj;
	vector<Matrix>	m_MatrixList;
	SMap*			m_Map;
	SQuadTree		m_QuadTree;
	bool			BoolColl;
	SCollision		Collision;
	SMousePicking	m_Mouse;
	SMiniMap		m_MinMap;
	SText			m_Textrue;
	SCamera			m_TopCamera;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	bool Build(int tel, int cel, int ces, wstring tex);
};

