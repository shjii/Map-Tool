#include "SLod.h"
void SLod::SetNeighvorNode(SNode* Node)
{
	SetNeighvor(Node);
	for (int i = 0; i < Node->m_ChildNode.size(); i++)
	{
		SetNeighvorNode(Node->m_ChildNode[i]);
	}
}
void SLod::SetNeighvor(SNode* Node)
{
	Node->m_NeghborList.resize(4);

	DWORD dwNumPatchCount = (DWORD)pow(2.0f, (float)Node->m_Depth);

	WORD dwNeighborCol, dwNeighborRow;
	if (Node->m_dwPositionIndex[1] > 0)  // 상
	{
		dwNeighborCol = Node->m_dwPositionIndex[0];
		dwNeighborRow = (Node->m_dwPositionIndex[1] - 1) * dwNumPatchCount;
		Node->m_NeghborList[0] = m_LevelLIst[Node->m_Depth][dwNeighborRow + dwNeighborCol];
	}
	if (Node->m_dwPositionIndex[1] < dwNumPatchCount - 1) // 하
	{
		dwNeighborCol = Node->m_dwPositionIndex[0];
		dwNeighborRow = (Node->m_dwPositionIndex[1] + 1) * dwNumPatchCount;
		Node->m_NeghborList[1] = m_LevelLIst[Node->m_Depth][dwNeighborRow + dwNeighborCol];
	}
	if (Node->m_dwPositionIndex[0] > 0) // 좌
	{
		dwNeighborCol = Node->m_dwPositionIndex[0] - 1;
		dwNeighborRow = Node->m_dwPositionIndex[1] * dwNumPatchCount;
		Node->m_NeghborList[2] = m_LevelLIst[Node->m_Depth][dwNeighborRow + dwNeighborCol];
	}
	if (Node->m_dwPositionIndex[0] < dwNumPatchCount - 1) // 우
	{
		dwNeighborCol = Node->m_dwPositionIndex[0] + 1;
		dwNeighborRow = Node->m_dwPositionIndex[1] * dwNumPatchCount;
		Node->m_NeghborList[3] = m_LevelLIst[Node->m_Depth][dwNeighborRow + dwNeighborCol];
	}
}
void	SLod::GetDrowList()
{
	m_LODDrawLIst.clear();

	for (int i = 0; i < m_DrawLIst.size(); i++)
	{
		AddDrowList(m_DrawLIst[i]);
	}
}
void SLod::AddDrowList(SNode* Node)
{
	if (Node->m_LeafNode == true)
	{
		m_LODDrawLIst.push_back(Node);
		GetLodSubIndex(Node);
		return;
	}

	for (int i = 0; i < Node->m_ChildNode.size(); i++)
	{
		AddDrowList(Node->m_ChildNode[i]);
	}
}
// Lod 레벨 추가 
DWORD SLod::GetLodSubIndex(SNode* Node)
{
	int iNumPatchIndex = m_iNumCell;
	float fRatio = GetExpansionRatio(Node->m_Box.vCenter);
	DWORD dwCurentRatio = (DWORD)(fRatio * m_iPatchLodCount);
	Node->m_LODLevel = dwCurentRatio;
	if ((DWORD)m_iPatchLodCount < Node->m_LODLevel)
	{
		Node->m_LODLevel -= 1;
	}
	return Node->m_LODLevel;
}
void  SLod::SetLOD(int iNumDepth)
{
	m_iNumCell = (int)((m_Map->m_iNumCellRows) / pow(2.0f, (float)iNumDepth));
	m_iPatchLodCount = (int)((log((float)m_iNumCell) / log(2.0f)));
}
float SLod::GetExpansionRatio(Vector3 vCenter)
{

	float fDistance = (m_mainCamera->m_vCameraPos - vCenter).Length();
	if (fDistance > m_mainCamera->m_fFarPlane)
	{
		return 1.0f;
	}
	float fRatio = fDistance / m_mainCamera->m_fFarPlane;
	return fRatio;
}

DWORD SLod::GetLodType(SNode* pNode)
{
	if (pNode->m_NeghborList.size() <= 0) return 0;
	DWORD dwType = 0;
	if (pNode->m_NeghborList[0] && pNode->m_NeghborList[0]->m_LODLevel < pNode->m_LODLevel)
	{dwType += 1;}
	if (pNode->m_NeghborList[1] && pNode->m_NeghborList[1]->m_LODLevel < pNode->m_LODLevel) 
	{dwType += 4;}
	if (pNode->m_NeghborList[2] && pNode->m_NeghborList[2]->m_LODLevel < pNode->m_LODLevel) 
	{dwType += 8;}
	if (pNode->m_NeghborList[3] && pNode->m_NeghborList[3]->m_LODLevel < pNode->m_LODLevel) 
	{dwType += 2;}

	pNode->m_LODType = dwType;
	return dwType;
}
int SLod::UpdateIndexList(SNode* pNode, DWORD dwCurentIndex, DWORD dwNumLevel)
{
	int iNumFaces = 0;

	DWORD dwTL = pNode->m_iCorner[0];
	DWORD dwTR = pNode->m_iCorner[1];
	DWORD dwBL = pNode->m_iCorner[2];
	DWORD dwBR = pNode->m_iCorner[3];

	DWORD dwStartRow = dwTL / m_Map->m_iNumRows;
	DWORD dwEndRow = dwBL / m_Map->m_iNumRows;

	DWORD dwStartCol = dwTL % m_Map->m_iNumRows;
	DWORD dwEndCol = dwTR % m_Map->m_iNumRows;

	DWORD dwOffset = (DWORD)(pow(2.0f, (float)dwNumLevel));
	DWORD dwCountX = ((dwEndCol - dwStartCol) / dwOffset) - 1;
	DWORD dwCountY = ((dwEndRow - dwStartRow) / dwOffset) - 1;


	DWORD dwYCell = 0;
	for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow += dwOffset, dwYCell++)
	{
		DWORD dwXCell = 0;
		for (DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol += dwOffset, dwXCell++)
		{
			//0	1    4   
			//2	   3 5
			DWORD dwNextRow = dwRow + dwOffset;
			DWORD dwNextCol = dwCol + dwOffset;
			if ((dwXCell == 0 && dwYCell == 0))
			{
				DWORD dwType = (pNode->m_LODType & 8) + (pNode->m_LODType & 1);
				iNumFaces += SetLodIndexBuffer(pNode, dwCurentIndex,
					dwNextRow * m_Map->m_iNumRows + dwCol,	// 2
					dwRow * m_Map->m_iNumRows + dwCol,		// 0
					dwRow * m_Map->m_iNumRows + dwNextCol,	// 1											
					dwType);
			}
			else if ((dwXCell == 0) && (pNode->m_LODType & 8))
			{
				iNumFaces += SetLodIndexBuffer(pNode, dwCurentIndex,
					dwNextRow * m_Map->m_iNumRows + dwCol,	// 2
					dwRow * m_Map->m_iNumRows + dwCol,		// 0
					dwRow * m_Map->m_iNumRows + dwNextCol,	// 1											
					8);
			}
			else if ((dwYCell == 0) && (pNode->m_LODType & 1))
			{
				iNumFaces += SetLodIndexBuffer(pNode, dwCurentIndex,
					dwNextRow * m_Map->m_iNumRows + dwCol,	// 2
					dwRow * m_Map->m_iNumRows + dwCol,		// 0
					dwRow * m_Map->m_iNumRows + dwNextCol,	// 1											
					1);
			}
			else
			{
				m_IndexList[dwCurentIndex + 0] = dwNextRow * m_Map->m_iNumRows + dwCol;
				m_IndexList[dwCurentIndex + 1] = dwRow * m_Map->m_iNumRows + dwCol;
				m_IndexList[dwCurentIndex + 2] = dwRow * m_Map->m_iNumRows + dwNextCol;
				iNumFaces += 1;
				dwCurentIndex += 3;
			}



			if ((dwXCell == dwCountX && dwYCell == dwCountY))
			{
				DWORD dwType = (pNode->m_LODType & 2) + (pNode->m_LODType & 4);
				iNumFaces += SetLodIndexBuffer(pNode, dwCurentIndex,
					dwRow * m_Map->m_iNumRows + dwNextCol,		// 2
					dwNextRow * m_Map->m_iNumRows + dwNextCol,	// 3
					dwNextRow * m_Map->m_iNumRows + dwCol,		// 0																					
					dwType);
			}
			else if ((dwXCell == dwCountX) && (pNode->m_LODType & 2))
			{
				iNumFaces += SetLodIndexBuffer(pNode, dwCurentIndex,
					dwRow * m_Map->m_iNumRows + dwNextCol,		// 2
					dwNextRow * m_Map->m_iNumRows + dwNextCol,	// 3
					dwNextRow * m_Map->m_iNumRows + dwCol,		// 0																					
					2);
			}
			else if ((dwYCell == dwCountY) && (pNode->m_LODType & 4))
			{
				iNumFaces += SetLodIndexBuffer(pNode, dwCurentIndex,
					dwRow * m_Map->m_iNumRows + dwNextCol,		// 2
					dwNextRow * m_Map->m_iNumRows + dwNextCol,	// 3
					dwNextRow * m_Map->m_iNumRows + dwCol,		// 0																					
					4);
			}
			else
			{
				m_IndexList[dwCurentIndex + 0] = dwRow * m_Map->m_iNumRows + dwNextCol;
				m_IndexList[dwCurentIndex + 1] = dwNextRow * m_Map->m_iNumRows + dwNextCol;
				m_IndexList[dwCurentIndex + 2] = dwNextRow * m_Map->m_iNumRows + dwCol;
				iNumFaces += 1;
				dwCurentIndex += 3;
			}
		}
	}
	return iNumFaces;
}
int  SLod::SetLodIndexBuffer(SNode* pNode, DWORD& dwCurentIndex, DWORD dwA, DWORD dwB, DWORD dwC, DWORD dwType)

{
	int iNumFaces = 0;

	if (dwType == 0)
	{
		m_IndexList[dwCurentIndex + 0] = dwA;
		m_IndexList[dwCurentIndex + 1] = dwB;
		m_IndexList[dwCurentIndex + 2] = dwC;
		iNumFaces += 1;
		dwCurentIndex += 3;
		return iNumFaces;
	}
	if (dwType == 8 || dwType == 2)// 좌우
	{
		DWORD dwCenter = (dwA + dwB) / 2;
		m_IndexList[dwCurentIndex + 0] = dwC;
		m_IndexList[dwCurentIndex + 1] = dwA;
		m_IndexList[dwCurentIndex + 2] = dwCenter;
		m_IndexList[dwCurentIndex + 3] = dwC;
		m_IndexList[dwCurentIndex + 4] = dwCenter;
		m_IndexList[dwCurentIndex + 5] = dwB;
		iNumFaces += 2;
		dwCurentIndex += 6;

		return iNumFaces;
	}
	if (dwType == 1 || dwType == 4)// 상하
	{
		DWORD dwCenter = (dwB + dwC) / 2;
		m_IndexList[dwCurentIndex + 0] = dwA;
		m_IndexList[dwCurentIndex + 1] = dwB;
		m_IndexList[dwCurentIndex + 2] = dwCenter;
		m_IndexList[dwCurentIndex + 3] = dwA;
		m_IndexList[dwCurentIndex + 4] = dwCenter;
		m_IndexList[dwCurentIndex + 5] = dwC;
		iNumFaces += 2;
		dwCurentIndex += 6;
		return iNumFaces;
	}
	if (dwType == 9 || dwType == 6)// 좌상, 우하
	{
		DWORD dwTopCenter = (dwB + dwC) / 2;
		DWORD dwLeftCenter = (dwA + dwB) / 2;

		m_IndexList[dwCurentIndex + 0] = dwLeftCenter;
		m_IndexList[dwCurentIndex + 1] = dwB;
		m_IndexList[dwCurentIndex + 2] = dwTopCenter;
		m_IndexList[dwCurentIndex + 3] = dwLeftCenter;
		m_IndexList[dwCurentIndex + 4] = dwTopCenter;
		m_IndexList[dwCurentIndex + 5] = dwA;
		m_IndexList[dwCurentIndex + 6] = dwA;
		m_IndexList[dwCurentIndex + 7] = dwTopCenter;
		m_IndexList[dwCurentIndex + 8] = dwC;
		iNumFaces += 3;
		dwCurentIndex += 9;
		return iNumFaces;
	}
	return iNumFaces;
}