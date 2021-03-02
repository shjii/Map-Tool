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
	// �ִ� Ʈ���� ���̸� ����Ѵ�.
	// m_dwWidth-1 = pow(2, Depth );
	// Depth       = log( (m_dwWidth-1) ) / log(2);
	// int iNumDepth      = log( (float)m_dwWidth-1 ) / log(2.0f);
	// m_LevelList.resize(iNumDepth+1);	

	// Ʈ���� ������ ���̿� ���� ��������� �ִ� ���� �ε��� ����(iNumLod)
	// BuildTree()�ȿ��� m_iMaxDepth�� ���� ����� Ʈ���� �ִ� ���̷� ����ȴ�.
	m_iNumCell = (int)((dwWidth - 1) / pow(2.0f, (float)iNumDepth));
	// LOD ���� ����( z = pow( x,y )���� y = log(z) / log(x) ) 
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


	// ����ȿ�� (����� LOD�� ���� �ٲ�� ���� �����Ѵ�.)
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
	///����ó��
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
// m_DrawNodeList���� ������常 m_DrawPatchNodeList�� �����Ѵ�.
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
