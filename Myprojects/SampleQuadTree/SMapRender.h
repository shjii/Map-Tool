#pragma once
#include "SQuadTree.h"
#include "SMap.h"
#include "SNode.h"

class SMapRender :	public SQuadTree
{
public:
	SMap*			m_pMap;
	PNCT_VERTEX*	m_pVertexList;
	vector<DWORD>	m_IndexList;
	ComPtr<ID3D11Buffer>	m_pTileIndexBuffer;
	int				m_iNumFace;
	int				m_iMinSplitDistance;
	DWORD			m_dwWidth;
	DWORD			m_dwHeight;
	bool			m_bSharedVertexBuffer;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
	size_t	m_dstibOffset;
	size_t	m_ibOffset;
public:
	bool					Build(SMap* pMap, DWORD dwWidth, DWORD dwHeight);
	SNode*					CreateNode(SNode* pParentNode, DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	Vector2					GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	void					ComputeBoungigBox(SNode* pNode);
	void					UpdateBoundingBox(SNode* pNode);
	bool					SubDivide(SNode* pNode);
	bool					Release();
	bool					Render(ID3D11DeviceContext*    pContext);
	bool					CreateVertexList(SNode* pNode);
	int						CreateIndexList(SNode* pNode);
public:
public:
	SMapRender();
	virtual ~SMapRender();
};