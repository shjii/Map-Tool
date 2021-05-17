#pragma once
#include "SQuadTree.h"
#include "SMap.h"
typedef vector<DWORD>	DWORD_VECTOR;
class SStaticData
{
public:
	DWORD					dwLevel;
	vector<DWORD_VECTOR>	IndexList;
	vector<ComPtr<ID3D11Buffer>>	IndexBufferList;
public:
	SStaticData();
	~SStaticData();
};

class SQuadTreeIndex : public SQuadTree
{
public:
	SMap*					m_pMap;
	PNCT_VERTEX*			m_pVertexList;
	DWORD*					m_IndexList;
	vector<SStaticData*>	m_pdwLodIndexArray;
	int						m_iNumFace;
	int						m_iMinSplitDistance;	// 최소 분할 크기( 정점간 간격 )
	DWORD					m_dwWidth;
	DWORD					m_dwHeight;
	bool					m_bDynamicUpdateIB;
	bool					m_bUsedCrossTypeIndex;
public:
	bool					Build(SMap* pMap, DWORD dwWidth, DWORD dwHeight);
	SNode*					CreateNode(SNode* pParenSNode, DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	int						UpdateIndexList(SNode* pNode, DWORD dwCurentIndex, DWORD dwLod);
	Vector2				GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	void					ComputeBoungigBox(SNode* pNode);
	void					UpdateBoundingBox(SNode* pNode);
	bool					SubDivide(SNode* pNode);
	bool					Frame();
	bool					Release();
	bool					Render(ID3D11DeviceContext*    pContext);
	bool					ComputeStaticLodIndex(int iSize);
	int						UpdateStaticIndexList(SNode* pNode, DWORD dwCurentIndex, DWORD dwLod);
	DWORD					GetIndex(DWORD dwIndex, DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR, DWORD dwLod);
	int						SetLodIndexBuffer(SNode* pNode,
		DWORD& dwCurentIndex,
		DWORD dwA, DWORD dwB, DWORD dwC,
		DWORD dwType);
	HRESULT			CreateIndexBuffer(SStaticData*  pData);
	DWORD			GetIndexCounter(DWORD dwData, DWORD dwQuadSize);
	HRESULT			CreateVertexBuffer(SNode* pNode, DWORD dwCount, PNCT_VERTEX* pvHeighSMap);
	bool			CreateVertexList(SNode* pNode);
public:
	SQuadTreeIndex(void);
	virtual ~SQuadTreeIndex(void);
};
