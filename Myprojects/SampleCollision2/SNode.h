#pragma once
#include "SFrustum.h"

class SNode
{
public:
	int							m_LODType;
	int							m_LODLevel;
	S_BOX						m_Box;
	S_SPHERE					m_Sphere;
	float						m_iCorner[4];
	vector<Vector3>				m_CornerList;
	vector<SNode*>				m_ChildNode;
	bool						m_LeafNode;
	std::vector<PNCT_VERTEX>	m_VertexList;
	std::vector<DWORD>			m_IndexList;
	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11Buffer*				m_pIndexBuffer;
	ID3D11Buffer*				m_pConstantBuffer;
	vector<SNode*>				m_NeghborList;
	int		m_Depth;
	DWORD				m_dwPositionIndex[2];// X,Y	
public:
	bool Release();
public:
	SNode();
};

