#pragma once
#include "SNode.h"
typedef vector<SNode*>	SNode_VECTOR;

class SMapLOD
{
public:
	ID3D11Device*			m_pd3dDevice;
	SCamera*				m_pCamera;
	vector<SNode_VECTOR>	m_LevelList;
	vector< SNode* >		m_DrawPatchNodeList;
	vector< SNode* >		m_DrawNodeList;
	// LOD 비율(  화면 투영 비율( 0 ~ 1 범위 사용 )
	float					m_fExpansionRatio;
	// 트리의 레벨의 깊이에 따른 셀 개수
	int						m_iNumCell;
	// LOD 레벨 개수( z = pow( x,y )에서 y = log(z) / log(x) ) 
	int						m_iPatchLodCount;
	bool					m_bThresHoldValue; // 문턱 값 사용
	void					SetThresHold(bool bFlag) { m_bThresHoldValue = bFlag; };

public:
	virtual void	InitLevelOrder(SNode* pRooSNode, int iMaxDepth);
	virtual void	SetLOD(DWORD dwWidth, int iNumDepth);
	virtual void	SetExpansionRatio(int iRatio) { m_fExpansionRatio = (float)iRatio; }
	virtual DWORD	GetLodSubIndex(SNode* pNode);
	virtual DWORD	GetLodType(SNode* pNode);
	virtual float	GetExpansionRatio(Vector3 vCenter, float fRadius);
	virtual float	GetExpansionRatio(Vector3 vMax, Vector3 vMin);
	virtual float	GetExpansionRatio(Vector3 vCenter);
	virtual void	GetDrawPatchNode();
	virtual void	AddDrawPatchNode(SNode* pNode);
	virtual void	Update(ID3D11Device* pd3dDevice, SCamera* pCamera = NULL);

public:
	SMapLOD(void);
	virtual ~SMapLOD(void);
};
