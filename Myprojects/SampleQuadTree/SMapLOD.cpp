#include "SMapLOD.h"

void  SMapLOD::InitLevelOrder(SNode* pRooSNode, int iMaxDepth)
{
	m_LevelList.resize(iMaxDepth + 1);
	for (int iLevel = 1; iLevel < iMaxDepth + 1; iLevel++)
	{
		m_LevelList[iLevel].resize(pow(4.0f, iLevel));
	}
	m_LevelList[0].push_back(pRooSNode);
}
void  SMapLOD::SetLOD(DWORD dwWidth, int iNumDepth)
{
	// 최대 트리의 깊이를 계산한다.
	// m_dwWidth-1 = pow(2, Depth );
	// Depth       = log( (m_dwWidth-1) ) / log(2);
	// int iNumDepth      = log( (float)m_dwWidth-1 ) / log(2.0f);
	// m_LevelList.resize(iNumDepth+1);	

	// 트리의 레벨의 깊이에 따른 리프노드의 최대 정점 인덱스 개수(iNumLod)
	// BuildTree()안에서 m_iMaxDepth은 실제 적용된 트리의 최대 깊이로 변경된다.
	m_iNumCell = (int)((dwWidth - 1) / pow(2.0f, (float)iNumDepth));
	// LOD 레벨 개수( z = pow( x,y )에서 y = log(z) / log(x) ) 
	m_iPatchLodCount = (int)((log((float)m_iNumCell) / log(2.0f)));
}

DWORD SMapLOD::GetLodSubIndex(SNode* pNode)
{
	int iNumPatchIndex = m_iNumCell;
	/*float fRatio = GetExpansionRatio(	pNode->m_tBox.vCenter,
										D3DXVec3Length( &(pNode->m_tBox.vMax-pNode->m_tBox.vCenter )));*/

										/*	float fRatio = GetExpansionRatio(	pNode->m_tBox.vMax, pNode->m_tBox.vMin);	*/

	//float fRatio = GetExpansionRatio(pNode->m_sBox.vCenter);
	DWORD dwCurentRatio = (DWORD)(1.0f * m_iPatchLodCount);
	DWORD dwMaxRatio = (DWORD)(1.0f * m_iPatchLodCount + 0.5f);
	DWORD dwMinRatio = (DWORD)(1.0f * m_iPatchLodCount - 0.5f);


	// 문턱효과 (노드의 LOD가 자주 바뀌는 것을 방지한다.)
	if (m_bThresHoldValue)
	{
		if (pNode->m_dwLodLevel < dwCurentRatio)
		{
			if (pNode->m_dwLodLevel < dwMinRatio)		// B
			{
				pNode->m_dwLodLevel = dwCurentRatio;
			}
		}
		else if (pNode->m_dwLodLevel > dwCurentRatio)
		{
			if (pNode->m_dwLodLevel > dwMaxRatio)		// B
			{
				pNode->m_dwLodLevel = dwCurentRatio;
			}
		}
	}
	else
	{
		pNode->m_dwLodLevel = dwCurentRatio;
	}
	///예외처리
	if ((DWORD)m_iPatchLodCount < pNode->m_dwLodLevel)
	{
		pNode->m_dwLodLevel -= 1;
	}

	return pNode->m_dwLodLevel;
}
DWORD SMapLOD::GetLodType(SNode* pNode)
{
	if (pNode->m_NeighborList.size() <= 0) return 0;
	DWORD dwType = 0;
	if (pNode->m_NeighborList[0] && pNode->m_NeighborList[0]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 1;
	if (pNode->m_NeighborList[1] && pNode->m_NeighborList[1]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 4;
	if (pNode->m_NeighborList[2] && pNode->m_NeighborList[2]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 8;
	if (pNode->m_NeighborList[3] && pNode->m_NeighborList[3]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 2;

	pNode->m_dwLodType = dwType;
	return dwType;
}
// m_DrawNodeList에서 리프노드만 m_DrawPatchNodeList에 저장한다.
void	SMapLOD::GetDrawPatchNode()
{
	for (int iNode = 0; iNode < m_DrawNodeList.size(); iNode++)
	{
		AddDrawPatchNode(m_DrawNodeList[iNode]);
	}
}
void SMapLOD::AddDrawPatchNode(SNode* pNode)
{
	if (pNode->m_isLeaf)
	{
		m_DrawPatchNodeList.push_back(pNode);
		GetLodSubIndex(pNode);
		return;
	}
	for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
	{
		AddDrawPatchNode(pNode->m_ChildList[iNode]);
	}
}
void SMapLOD::Update(ID3D11Device* pd3dDevice, SCamera* pCamera)
{
	m_pd3dDevice = pd3dDevice;
	m_pCamera = pCamera;
}
SMapLOD::SMapLOD(void)
{
	m_fExpansionRatio = 0.1f;
	m_iNumCell = 1;
	m_iPatchLodCount = 0;
	m_pCamera = NULL;
	m_bThresHoldValue = false;
}

SMapLOD::~SMapLOD(void)
{
}
