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
float SMapLOD::GetExpansionRatio(Vector3 vCenter, float fRadius)
{
	float fRatio = 0.0f;
	Vector3 vCenterViewPos, vSideViewPos;
	vSideViewPos = vCenter + (m_pCamera->m_vRight * fRadius);
	vCenterViewPos = XMVector3TransformCoord(vCenter, m_pCamera->m_matView);
	vSideViewPos = XMVector3TransformCoord(vSideViewPos, m_pCamera->m_matView);

	Vector3 vCenterProjPos, vSideProjPos;
	vCenterProjPos = XMVector3TransformCoord(vCenterViewPos, m_pCamera->m_matProj);
	vSideProjPos = XMVector3TransformCoord(vSideViewPos, m_pCamera->m_matProj);

	vCenterProjPos.Normalize();
	Vector2 vA = Vector2(vCenterProjPos.x * 0.5f + 0.5f, vCenterProjPos.y * 0.5f + 0.5f);
	Vector2 vB = Vector2(vSideProjPos.x* 0.5f + 0.5f, vSideProjPos.y* 0.5f + 0.5f);

	//TViewPort m_MainVP;
	//m_MainVP.Set( 0, 0, 800, 600, 0.0f, 1.0f );
	//D3DXVec3Project( &vCenterProjPos, &vCenter,&m_MainVP.m_ViewPort,m_pCamera->m_matProj, m_pCamera->m_matView, 0 );
	//D3DXVec3Unproject(&vCenterProjPos, &vCenterProjPos,&m_MainVP.m_ViewPort,m_pCamera->m_matProj, m_pCamera->m_matView, 0 );
	//fRatio = D3DXVec2Length( &(Vector2(0.0f,0.0f) - Vector2(1.0f,1.0f)));
	//fRatio = vCenterProjPos.z;//D3DXVec2Length( &(Vector2(vA-vB)));
	// 카메라가 지형에서 멀리 떨어질 경우에 해당함.
	float fDistance = (m_pCamera->m_vCameraPos - vCenter).Length();
	if (fDistance > m_pCamera->m_fFarPlane)
	{
		return 1.0f;
	}
	fRatio = 1.0f - fDistance / m_pCamera->m_fFarPlane;
	return fRatio;
}
float SMapLOD::GetExpansionRatio(Vector3 vCenter)
{
	float fDistance = (m_pCamera->m_vCameraPos - vCenter).Length();
	if (fDistance > m_pCamera->m_fFarPlane)
	{
		return 1.0f;
	}
	float fRatio = fDistance / m_pCamera->m_fFarPlane;
	return fRatio;
}
float SMapLOD::GetExpansionRatio(Vector3 vMax, Vector3 vMin)
{
	float fRatio = 0.0f;
	Vector3 vCenterViewPos, vSideViewPos;

	//D3DXMATRIX matInverse = *m_pCamera->m_matView;
	//D3DXMatrixInverse( &matInverse, 0, &matInverse);
	//matInverse._41 = 0.0f;
	//matInverse._42 = 0.0f;
	//matInverse._43 = 0.0f;
	//matInverse._44 = 1.0f;
	//D3DXVec3TransformCoord( &vMax, &vMax, &matInverse );		
	//D3DXVec3TransformCoord( &vMin, &vMin, &matInverse );

	vCenterViewPos = XMVector3TransformCoord(vMax, m_pCamera->m_matView);
	vSideViewPos = XMVector3TransformCoord(vMin, m_pCamera->m_matView);

	Vector3 vCenterProjPos, vSideProjPos;
	vCenterProjPos = XMVector3TransformCoord(vCenterViewPos, m_pCamera->m_matProj);
	vSideProjPos = XMVector3TransformCoord(vSideViewPos, m_pCamera->m_matProj);

	Vector2 vA = Vector2(vCenterProjPos.x * 0.5f + 0.5f, vCenterProjPos.y * 0.5f + 0.5f);
	Vector2 vB = Vector2(vSideProjPos.x* 0.5f + 0.5f, vSideProjPos.y* 0.5f + 0.5f);

	fRatio = (vCenterProjPos - vSideProjPos).Length();
	//fRatio = D3DXVec2Length( &(Vector2(vA-vB)));
	return fRatio;
}
DWORD SMapLOD::GetLodSubIndex(SNode* pNode)
{
	int iNumPatchIndex = m_iNumCell;
	/*float fRatio = GetExpansionRatio(	pNode->S_BOX.vCenter,
										D3DXVec3Length( &(pNode->S_BOX.vMax-pNode->S_BOX.vCenter )));*/

										/*	float fRatio = GetExpansionRatio(	pNode->S_BOX.vMax, pNode->S_BOX.vMin);	*/

	float fRatio = GetExpansionRatio(pNode->S_BOX.vCenter);
	DWORD dwCurentRatio = (DWORD)(fRatio * m_iPatchLodCount);
	DWORD dwMaxRatio = (DWORD)(fRatio * m_iPatchLodCount + 0.5f);
	DWORD dwMinRatio = (DWORD)(fRatio * m_iPatchLodCount - 0.5f);


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
