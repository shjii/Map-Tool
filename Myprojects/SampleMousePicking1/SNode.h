#pragma once
#include "SFrustum.h"

class SNode
{
public:
	S_BOX						m_Box;
	float						m_iCorner[4];
	vector<Vector3>				m_CornerList;
	vector<SNode*>				m_ChildNode;
	bool						m_LeafNode;
	std::vector<PNCT_VERTEX>	m_VertexList;
	std::vector<DWORD>			m_IndexList;
	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11Buffer*				m_pIndexBuffer;
	ID3D11Buffer*				m_pConstantBuffer;
	int		m_Depth;
public:
	SNode();
	~SNode();
};
