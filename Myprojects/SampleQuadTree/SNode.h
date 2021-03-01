#pragma once
#include "SFrustum.h"

class SBaseObj
{
public:
	S_BOX	m_sBox;
public:
	SBaseObj() {}
	virtual ~SBaseObj() {}
};

class S_BoxObject : public SBaseObj
{
public:
	Matrix	m_matWorld;
	Vector3	m_vPosition;
	Vector4	m_vColor;
public:
	S_BoxObject();
};

class SNode
{
public:
	DWORD			m_dwDepth;
	BOOL			m_isLeaf;
	BOOL			m_isTile;

	S_BOX			m_sBox;
	vector<DWORD>	m_CornerIndex;
	// ����� �ڳ� ���� �ε��� �� ��ġ
	vector<Vector3>		m_CornerList;
	vector<SNode*>		m_ChildList;
	vector<SNode*>		m_NeighborList;
	vector<SBaseObj*>	m_ObjectList;
	// �߰�
	DWORD				m_dwPositionIndex[2];// X,Y	
	D3D11_BOX			m_BoxVB;
	D3D11_BOX			m_BoxIB;
	UINT				m_iBeginVB;
	UINT				m_iBeginIB;
	vector<DWORD>		m_IndexList;
	vector <PNCT_VERTEX>	m_VertexList;
	ComPtr<ID3D11Buffer>	m_pVertexBuffer;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
public:
	SNode();
	virtual ~SNode();
};

