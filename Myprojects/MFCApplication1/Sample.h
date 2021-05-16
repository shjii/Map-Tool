#pragma once
#include "TCore.h"
#include "SMousePicking.h"
#include "FileIO.h"
enum MapEditorBuffer
{
	UP = 0,
	DOWN ,
	NORMAL ,

	ELayer0,
	ELayer1,
	ELayer2,
	ELayer3,
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
	FileIO			FileIO;
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
	vector<Vector3> m_PinkList;
	vector<ComPtr<ID3D11ShaderResourceView>> m_MultiTextureSRV;
	MapFileData		m_MapData;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	bool Build(int tel, int cel, int ces, wstring tex);
	bool GetIntersection(SNode* pNode);
	bool SetEditor();
	bool SetLayer();
public:
	Sample();
	~Sample();
};

