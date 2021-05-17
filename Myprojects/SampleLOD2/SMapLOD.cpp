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
	// ī�޶� �������� �ָ� ������ ��쿡 �ش���.
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
