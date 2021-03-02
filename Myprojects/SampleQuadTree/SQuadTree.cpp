#include "SQuadTree.h"
bool SQuadTree::Build(float fWidth, float fHeight)
{
	m_fWidth = fWidth, m_fHeight = fHeight;
	m_pRooSNode = CreateNode(NULL, -fWidth / 2, fWidth / 2, -fHeight / 2, fHeight / 2);

	if (BuildTree(m_pRooSNode))
	{
		return true;
	}
	return false;
}
bool SQuadTree::BuildTree(SNode* pNode)
{
	// ���� �����ϳ�
	if (SubDivide(pNode))
	{
		for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
		{
			/*		if (m_bUsedIndexList)
					{
						DWORD dwIndex =
							pNode->m_ChildList[iNode]->m_dwPositionIndex[1] * pow(2.0f, (float)pNode->m_ChildList[iNode]->m_dwDepth) + pNode->m_ChildList[iNode]->m_dwPositionIndex[0];
						DWORD dwValue = pNode->m_ChildList[iNode]->m_dwDepth;
						m_LevelList[dwValue][dwIndex] =
							pNode->m_ChildList[iNode];
					}*/
			BuildTree(pNode->m_ChildList[iNode]);
		}
	}
	return true;
}
bool SQuadTree::SubDivide(SNode* pNode)
{
	// 4��� �Ҽ� ������ ���̻� �������� �ʴ´�.
	if (pNode == NULL)
	{
		return false;
	}

	// �ִ� ���� �ѵ� �ʰ��� ���� ������� ����
	if ((DWORD)m_iMaxDepthLimit <= pNode->m_dwDepth)
	{
		pNode->m_isLeaf = TRUE;
		return false;
	}

	// ���� ����� ���� ũ�⸦ ����Ѵ�.

	float fWidthSplit = (pNode->m_CornerList[tTR].x - pNode->m_CornerList[tTL].x) / 2;
	float fHeightSplit = (pNode->m_CornerList[tTL].z - pNode->m_CornerList[tBL].z) / 2;

	// �ڽ� ��尡 ������ ����ũ�⺸�� �۴ٸ� ���̻� �������� �ʴ´�.
	if (fWidthSplit < m_fMinDivideSize || fHeightSplit < m_fMinDivideSize)
	{
		pNode->m_isLeaf = TRUE;
		return false;
	}

	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_CornerList[tTL].x,
		pNode->m_CornerList[tTL].x + fWidthSplit,
		pNode->m_CornerList[tTL].z - fHeightSplit,
		pNode->m_CornerList[tTL].z));

	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_CornerList[tTL].x + fWidthSplit,
		pNode->m_CornerList[tTR].x,
		pNode->m_CornerList[tTL].z - fHeightSplit,
		pNode->m_CornerList[tTL].z));

	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_CornerList[tTL].x,
		pNode->m_CornerList[tTL].x + fWidthSplit,
		pNode->m_CornerList[tBL].z,
		pNode->m_CornerList[tTL].z - fHeightSplit));

	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_CornerList[tTL].x + fWidthSplit,
		pNode->m_CornerList[tTR].x,
		pNode->m_CornerList[tBR].z,
		pNode->m_CornerList[tTL].z - fHeightSplit));


	return true;
}
SNode* SQuadTree::CreateNode(SNode* pParenSNode, float fTopLeft, float fTopRight, float fBottomLeft, float fBottomRight)
{
	SNode* pNode = 0;
	SAFE_NEW(pNode, SNode);
	assert(pNode);

	pNode->m_ChildList.reserve(4);
	pNode->m_CornerList.reserve(4);

	pNode->m_sBox.vMin = Vector3(fTopLeft, 0.0f, fBottomLeft);
	pNode->m_sBox.vMax = Vector3(fTopRight, 0.0f, fBottomRight);
	pNode->m_sBox.vCenter = (pNode->m_sBox.vMax + pNode->m_sBox.vMin);
	pNode->m_sBox.vCenter.x /= 2.0f;
	pNode->m_sBox.vCenter.y /= 2.0f;
	pNode->m_sBox.vCenter.z /= 2.0f;
	pNode->m_sBox.fExtent[0] = pNode->m_sBox.vMax.x - pNode->m_sBox.vCenter.x;
	pNode->m_sBox.fExtent[1] = pNode->m_sBox.vMax.y - pNode->m_sBox.vCenter.y;
	pNode->m_sBox.fExtent[2] = pNode->m_sBox.vMax.z - pNode->m_sBox.vCenter.z;

	pNode->m_CornerList.push_back(Vector3(pNode->m_sBox.vMin.x, 0.0f, pNode->m_sBox.vMax.z));
	pNode->m_CornerList.push_back(pNode->m_sBox.vMax);
	pNode->m_CornerList.push_back(pNode->m_sBox.vMin);
	pNode->m_CornerList.push_back(Vector3(pNode->m_sBox.vMax.x, 0.0f, pNode->m_sBox.vMin.z));

	if (pParenSNode)
	{
		pNode->m_dwDepth = pParenSNode->m_dwDepth + 1;
		if ((DWORD)m_iMaxDepth < pNode->m_dwDepth)
		{
			m_iMaxDepth = pNode->m_dwDepth;
		}
	}
	return pNode;
}

int SQuadTree::AddObject(SBaseObj* pObj)
{
	if (CheckRect(m_pRooSNode, pObj))
	{
		SNode* pNode = FindNode(m_pRooSNode, pObj);
		if (pNode)
		{
			pNode->m_ObjectList.push_back(pObj);
		}
		return 1;
	}
	return 0;
}
int SQuadTree::CheckRect(SNode* pNode, SBaseObj* pObj)
{
	if (pNode->m_sBox.vMin.x <= pObj->m_sBox.vMin.x && pNode->m_sBox.vMax.x >= pObj->m_sBox.vMax.x)
	{
		if (pNode->m_sBox.vMin.z <= pObj->m_sBox.vMin.z && pNode->m_sBox.vMax.z >= pObj->m_sBox.vMax.z)
		{
			return 1;
		}
	}
	return 0;
}
SNode*	SQuadTree::FindNode(SNode* pNode, SBaseObj* pObj)
{
	assert(pNode);
	do {
		for (DWORD dwChild = 0; dwChild < pNode->m_ChildList.size(); dwChild++)
		{
			if (pNode->m_ChildList[dwChild] &&
				CheckRect(pNode->m_ChildList[dwChild], pObj))
			{
				m_QuadTreeQueue.push(pNode->m_ChildList[dwChild]);
				break;
			}
		}

		if (m_QuadTreeQueue.empty()) break;

		pNode = m_QuadTreeQueue.front();
		m_QuadTreeQueue.pop();
	} while (pNode);
	return pNode;
}
bool SQuadTree::PreFrame()
{
	m_DrawObjList.clear();
	m_DrawNodeList.clear();
	//if (!m_pCamera) return false;
	return true;
}
bool SQuadTree::Frame()
{
	if (!PreFrame()) return false;
	DrawFindNode(m_pRooSNode);
	return true;
}

void SQuadTree::VisibleNode(SNode* pNode)
{
	////assert(m_pCamera);
	//if (pNode->m_dwDepth < (DWORD)m_iRenderDepth) return;

	//if (m_pCamera->CheckOBBInPlane(&pNode->m_sBox))
	//{
	//	VisibleObject(pNode);
	//	for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
	//	{
	//		VisibleNode(pNode->m_ChildList[iNode]);
	//	}
	//}
}
void SQuadTree::VisibleObject(SNode* pNode)
{
	for (int iObj = 0; iObj < pNode->m_ObjectList.size(); iObj++)
	{
		/*if (m_pCamera->CheckOBBInPlane(&pNode->m_ObjectList[iObj]->m_sBox))
		{
			m_DrawObjList.push_back(pNode->m_ObjectList[iObj]);
		}*/
	}
}
void SQuadTree::DrawFindNode(SNode* pNode)
{
	//assert(pNode);

	//T_POSITION t_Pos = m_pCamera->CheckPoitionOBBInPlane(&pNode->m_sBox);
	//if (pNode->m_dwDepth < (DWORD)m_iRenderDepth ||
	//	t_Pos == P_BACK)
	//{
	//	return;
	//}
	//GetLodSubIndex(pNode);

	//// ������� �� ���� ������ ���ܵ��� �ʾҴٸ�(���� �ְų� ���� ����)�߰��Ѵ�.
	//if (pNode->m_isLeaf &&  t_Pos != P_BACK)
	//{
	//	m_DrawNodeList.push_back(pNode);
	//	VisibleObject(pNode);
	//	return;
	//}
	//// ������ ���ԵǾ� ������ �ڽ��� Ž������ �ʰ� ��带 �߰��Ѵ�.
	//if (t_Pos == P_FRONT)
	//{
	//	m_DrawNodeList.push_back(pNode);
	//	VisibleNode(pNode);
	//	return;
	//}

	//// ������ �ִ� ��忡 ���Ե� ������Ʈ üũ
	//if (t_Pos == P_SPANNING)
	//{
	//	VisibleObject(pNode);
	//}

	//for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
	//{
	//	DrawFindNode(pNode->m_ChildList[iNode]);
	//}
}
void SQuadTree::SetRenderDepth(DWORD dwDepth)
{
	/*if ((DWORD)m_iMaxDepth <= dwDepth) m_iRenderDepth = -1;
	else						m_iRenderDepth = dwDepth;*/
}
void SQuadTree::FindNeighborNode(SNode* pNode)
{
	//pNode->m_NeighborList.resize(4);

	//DWORD dwNumPatchCount = (DWORD)pow(2.0f, (float)pNode->m_dwDepth);

	//DWORD dwNeighborCol, dwNeighborRow;
	//if (pNode->m_dwPositionIndex[1] > 0)  // ��
	//{
	//	dwNeighborCol = pNode->m_dwPositionIndex[0];
	//	dwNeighborRow = (pNode->m_dwPositionIndex[1] - 1) * dwNumPatchCount;
	//	pNode->m_NeighborList[0] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	//}
	//if (pNode->m_dwPositionIndex[1] < dwNumPatchCount - 1) // ��
	//{
	//	dwNeighborCol = pNode->m_dwPositionIndex[0];
	//	dwNeighborRow = (pNode->m_dwPositionIndex[1] + 1) * dwNumPatchCount;
	//	pNode->m_NeighborList[1] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	//}
	//if (pNode->m_dwPositionIndex[0] > 0) // ��
	//{
	//	dwNeighborCol = pNode->m_dwPositionIndex[0] - 1;
	//	dwNeighborRow = pNode->m_dwPositionIndex[1] * dwNumPatchCount;
	//	pNode->m_NeighborList[2] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	//}
	//if (pNode->m_dwPositionIndex[0] < dwNumPatchCount - 1) // ��
	//{
	//	dwNeighborCol = pNode->m_dwPositionIndex[0] + 1;
	//	dwNeighborRow = pNode->m_dwPositionIndex[1] * dwNumPatchCount;
	//	pNode->m_NeighborList[3] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	//}
}
void SQuadTree::SetNeighborNode(SNode* pNode)
{
	FindNeighborNode(pNode);
	for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
	{
		SetNeighborNode(pNode->m_ChildList[iNode]);
	}
}
DWORD SQuadTree::CheckSize(DWORD dwSize)
{
	float fCount = 0;
	DWORD dwChecker = dwSize / 2;
	if (dwChecker <= 0) return 0;

	while (dwChecker != 0)
	{
		dwChecker >>= 1;
		fCount += 1.0f;
	}
	float fMinimum = pow(2.0f, fCount - 1.0f);
	float fMaximum = pow(2.0f, fCount);
	// fMin = abs( fMin - P0 ) - abs( P1 - fMin );
	// fMax = abs( fMax - P0 ) - abs( P1 - fMax );
	// return min( fMin, fMax );
	float fMin = fabs(fMinimum - fabs(((float)dwSize - fMinimum)));
	float fMax = fabs(fMaximum - fabs(((float)dwSize - fMaximum)));

	// �񱳰��� ���ٸ� ���� ����ũ�⸦ ��ȯ�Ѵ�. ��) fMin=1 : fMax=1 => fMinimum=2 : fMaximum=4
	if (fMin <= fMax)
	{
		return (DWORD)fMinimum;
	}
	return (DWORD)fMaximum;
}
bool SQuadTree::Release()
{
	SAFE_DEL(m_pRooSNode);
	return true;
}


SQuadTree::SQuadTree(void)
{
	m_iRenderDepth = 0;
	m_iMaxDepth = 0;
	m_iMaxDepthLimit = 1;
	m_fMinDivideSize = 2;
	m_bUsedIndexList = false;
	m_pd3dDevice = 0;
	m_pRooSNode = 0;
}

SQuadTree::~SQuadTree(void)
{
	Release();
}