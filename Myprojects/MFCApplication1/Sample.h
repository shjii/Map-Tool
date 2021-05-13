#pragma once
#include "TCore.h"
#include "SQuadTree.h"
#include "SModel.h"
#include "SMousePicking.h"
#include "SMiniMap.h"
#include "SText.h"
enum MapEditorBuffer
{
	UP = 0,
	DOWN ,
	NORMAL ,
};
struct MapEditorData
{
	float Radius;
	float Speed;
	MapEditorBuffer mapEditorB;
};
class Sample : public TCore
{
public:
	ComPtr<ID3D11Buffer>			m_ConstantBuffer;
	SModel			m_pObj;
	vector<Matrix>	m_MatrixList;
	SMap*			m_Map;
	SQuadTree		m_QuadTree;
	bool			BoolColl;
	SMousePicking	m_Mouse;
	SMiniMap		m_MinMap;
	SText			m_Textrue;
	SText			m_BlendingTextrue;
	SCamera			m_TopCamera;
	MapEditorData	m_EditorData;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	bool Build(int tel, int cel, int ces, wstring tex);
	bool GetIntersection(SNode* pNode);
};

