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
	// 분할 가능하냐
	if (SubDivide(pNode))
	{
		for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
		{
			if (m_bUsedIndexList)
			{
				DWORD dwIndex =
					pNode->m_ChildList[iNode]->m_dwPositionIndex[1] * pow(2.0f, (float)pNode->m_ChildList[iNode]->m_dwDepth) + pNode->m_ChildList[iNode]->m_dwPositionIndex[0];
				DWORD dwValue = pNode->m_ChildList[iNode]->m_dwDepth;
				m_LevelList[dwValue][dwIndex] =
					pNode->m_ChildList[iNode];
			}
			BuildTree(pNode->m_ChildList[iNode]);
		}
	}
	return true;
}
bool SQuadTree::SubDivide(SNode* pNode)
{
	// 4등분 할수 없으면 더이상 분할하지 않는다.
	if (pNode == NULL)
	{
		return false;
	}

	// 최대 깊이 한도 초과시 강제 리프노드 지정
	if ((DWORD)m_iMaxDepthLimit <= pNode->m_dwDepth)
	{
		pNode->m_isLeaf = TRUE;
		return false;
	}

	// 현재 노드의 실제 크기를 계산한다.

	float fWidthSplit = (pNode->m_CornerList[tTR].x - pNode->m_CornerList[tTL].x) / 2;
	float fHeightSplit = (pNode->m_CornerList[tTL].z - pNode->m_CornerList[tBL].z) / 2;

	// 자식 노드가 지정된 분할크기보다 작다면 더이상 분할하지 않는다.
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

	pNode->S_BOX.vMin = Vector3(fTopLeft, 0.0f, fBottomLeft);
	pNode->S_BOX.vMax = Vector3(fTopRight, 0.0f, fBottomRight);
	pNode->S_BOX.vCenter = (pNode->S_BOX.vMax + pNode->S_BOX.vMin) / 2.0f;
	pNode->S_BOX.fExtent[0] = pNode->S_BOX.vMax.x - pNode->S_BOX.vCenter.x;
	pNode->S_BOX.fExtent[1] = pNode->S_BOX.vMax.y - pNode->S_BOX.vCenter.y;
	pNode->S_BOX.fExtent[2] = pNode->S_BOX.vMax.z - pNode->S_BOX.vCenter.z;

	pNode->m_CornerList.push_back(Vector3(pNode->S_BOX.vMin.x, 0.0f, pNode->S_BOX.vMax.z));
	pNode->m_CornerList.push_back(pNode->S_BOX.vMax);
	pNode->m_CornerList.push_back(pNode->S_BOX.vMin);
	pNode->m_CornerList.push_back(Vector3(pNode->S_BOX.vMax.x, 0.0f, pNode->S_BOX.vMin.z));

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

int SQuadTree::AddObject(TObject* pObj)
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
int SQuadTree::CheckRect(SNode* pNode, TObject* pObj)
{
	if (pNode->S_BOX.vMin.x <= pObj->m_Box.vMin.x && pNode->S_BOX.vMax.x >= pObj->m_Box.vMax.x)
	{
		if (pNode->S_BOX.vMin.z <= pObj->m_Box.vMin.z && pNode->S_BOX.vMax.z >= pObj->m_Box.vMax.z)
		{
			return 1;
		}
	}
	return 0;
}
SNode*	SQuadTree::FindNode(SNode* pNode, TObject* pObj)
{
	assert(pNode);
	do {
		for (DWORD dwChild = 0; dwChild < pNode->m_ChildList.size(); dwChild++)
		{
			if (pNode->m_ChildList[dwChild] && CheckRect(pNode->m_ChildList[dwChild], pObj))
			{
				m_QuadTreeQueue.push(pNode->m_ChildList[dwChild]);
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
	if (!m_pCamera) return false;
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
	assert(m_pCamera);
	if (pNode->m_dwDepth < (DWORD)m_iRenderDepth) return;

	if (m_pCamera->CheckOBBInPlane(&pNode->S_BOX))
	{
		VisibleObject(pNode);
		for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
		{
			VisibleNode(pNode->m_ChildList[iNode]);
		}
	}
}
void SQuadTree::VisibleObject(SNode* pNode)
{
	for (int iObj = 0; iObj < pNode->m_ObjectList.size(); iObj++)
	{
		if (m_pCamera->CheckOBBInPlane(&pNode->m_ObjectList[iObj]->m_Box))
		{
			m_DrawObjList.push_back(pNode->m_ObjectList[iObj]);
		}
	}
}
void SQuadTree::DrawFindNode(SNode* pNode)
{
	assert(pNode);

	S_POSITION t_Pos = m_pCamera->CheckPoitionOBBInPlane(&pNode->S_BOX);
	GetLodSubIndex(pNode);
	if (pNode->m_dwDepth < (DWORD)m_iRenderDepth) return;

	// 리프노드 일 경우는 완전히 제외되지 않았다면(걸쳐 있거나 완전 포함)추가한다.
	if (pNode->m_isLeaf &&  t_Pos != P_BACK)
	{
		m_DrawNodeList.push_back(pNode);
		VisibleObject(pNode);
		return;
	}
	// 완전히 포함되어 있으면 자식을 탐색하지 않고 노드를 추가한다.
	if (t_Pos == P_FRONT)
	{
		m_DrawNodeList.push_back(pNode);
		VisibleNode(pNode);
		return;
	}

	// 걸쳐져 있는 노드에 포함된 오브젝트 체크
	if (t_Pos == P_SPANNING)
	{
		VisibleObject(pNode);
	}

	for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
	{
		DrawFindNode(pNode->m_ChildList[iNode]);
	}
}
void SQuadTree::SetRenderDepth(DWORD dwDepth)
{
	if ((DWORD)m_iMaxDepth <= dwDepth) m_iRenderDepth = -1;
	else						m_iRenderDepth = dwDepth;
}
void SQuadTree::FindNeighborNode(SNode* pNode)
{
	pNode->m_NeighborList.resize(4);

	DWORD dwNumPatchCount = (DWORD)pow(2.0f, (float)pNode->m_dwDepth);

	DWORD dwNeighborCol, dwNeighborRow;
	if (pNode->m_dwPositionIndex[1] > 0)  // 상
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0];
		dwNeighborRow = (pNode->m_dwPositionIndex[1] - 1) * dwNumPatchCount;
		pNode->m_NeighborList[0] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	}
	if (pNode->m_dwPositionIndex[1] < dwNumPatchCount - 1) // 하
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0];
		dwNeighborRow = (pNode->m_dwPositionIndex[1] + 1) * dwNumPatchCount;
		pNode->m_NeighborList[1] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	}
	if (pNode->m_dwPositionIndex[0] > 0) // 좌
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0] - 1;
		dwNeighborRow = pNode->m_dwPositionIndex[1] * dwNumPatchCount;
		pNode->m_NeighborList[2] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	}
	if (pNode->m_dwPositionIndex[0] < dwNumPatchCount - 1) // 우
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0] + 1;
		dwNeighborRow = pNode->m_dwPositionIndex[1] * dwNumPatchCount;
		pNode->m_NeighborList[3] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	}
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

	// 비교값이 같다면 작은 분할크기를 반환한다. 예) fMin=1 : fMax=1 => fMinimum=2 : fMaximum=4
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
	m_iNumCell = 1;
	m_iPatchLodCount = 0;
	m_iRenderDepth = 0;
	m_iMaxDepth = 0;
	m_iMaxDepthLimit = 4;
	m_fMinDivideSize = 2;
	m_bUsedIndexList = false;
	SAFE_ZERO(m_pd3dDevice);
	SAFE_ZERO(m_pRooSNode);
}

SQuadTree::~SQuadTree(void)
{
	Release();
}
