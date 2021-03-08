#include "SQuadTree.h"
bool	SQuadTree::Build(SMap* m_Map)
{
	this->m_Map = m_Map;

	Face = m_Map->m_iNumCellCols * m_Map->m_iNumCellRows * 2;
	m_IndexList.resize(Face * 2 * 3);
	m_RootNode = BuildNode(m_Map);
	SetLOD(m_MaxDepth);
	BoungigBox(m_RootNode);
	m_LevelLIst.resize(m_MaxDepth + 1);
	m_LevelLIst[0].push_back(m_RootNode);
	if (Division(m_RootNode))
	{
		SetNeighvorNode(m_RootNode);
	}
	return true;
}

SNode * SQuadTree::BuildNode(SMap * m_Map)
{
	SNode* sNode = new SNode;
	sNode->m_Depth = 0;
	sNode->m_iCorner[TL] = 0;
	sNode->m_iCorner[TR] = m_Map->m_iNumCols - 1 ;
	sNode->m_iCorner[BL] = m_Map->m_iNumVertices - m_Map->m_iNumCols;
	sNode->m_iCorner[BR] = m_Map->m_iNumVertices - 1;

	sNode->m_IndexList = m_Map->m_IndexList;
	sNode->m_VertexList = m_Map->m_VertexList;
	sNode->m_pIndexBuffer = m_Map->m_pIndexBuffer;
	sNode->m_pVertexBuffer = m_Map->m_pVertexBuffer;
	return sNode;
}
bool	SQuadTree::Division(SNode* Node)
{
	if (Node == NULL) return false;

	BoungigBox(Node);

	if (Node->m_Depth >= m_MaxDepth)
	{
		CreatorIndexBuffer(Node, m_Map->m_iNumCols, m_Map->m_iNumRows);
		Node->m_LeafNode = true;
		return false;
	}

	float centerCol = (Node->m_iCorner[TR] - Node->m_iCorner[TL]) / 2;
	float centerRow = (Node->m_iCorner[BL] - Node->m_iCorner[TL]) / 2;

	// 밑으로 row 옆으로 col
	//Node->m_ChildNode.push_back(CreatorNode(Node, Node->m_iCorner[TL], Node->m_iCorner[TL] + centerCol, Node->m_iCorner[TL] + centerRow, Node->m_iCorner[TL] + centerRow + centerCol));
	//Node->m_ChildNode.push_back(CreatorNode(Node, Node->m_iCorner[TL] + centerCol, Node->m_iCorner[TR], Node->m_iCorner[TL] + centerRow + centerCol, Node->m_iCorner[TR] + centerRow));
	//Node->m_ChildNode.push_back(CreatorNode(Node, Node->m_iCorner[TL] + centerRow, Node->m_iCorner[TL] + centerRow + centerCol, Node->m_iCorner[BL], Node->m_iCorner[BL] + centerCol));
	//Node->m_ChildNode.push_back(CreatorNode(Node, Node->m_iCorner[TL] + centerRow + centerCol, Node->m_iCorner[TR] + centerRow, Node->m_iCorner[BL] + centerCol, Node->m_iCorner[BR]));
	
	float fWidthSplit = (Node->m_iCorner[1] - Node->m_iCorner[0]) / 2;
	float fHeightSplit = (Node->m_iCorner[3] - Node->m_iCorner[2]) / 2;
	UINT e0 = Node->m_iCorner[0] + fWidthSplit;
	UINT e1 = Node->m_iCorner[0] + (m_Map->m_iNumCols * fHeightSplit);
	UINT e2 = e0 + (m_Map->m_iNumCols * fHeightSplit);
	UINT e3 = Node->m_iCorner[1] + (m_Map->m_iNumCols * fHeightSplit);;
	UINT e4 = Node->m_iCorner[3] - fHeightSplit;
	
	Node->m_ChildNode.push_back(CreatorNode(Node, Node->m_iCorner[0], e0, e1, e2));
	Node->m_ChildNode.push_back(CreatorNode(Node, e0, Node->m_iCorner[1], e2, e3));
	Node->m_ChildNode.push_back(CreatorNode(Node, e1, e2, Node->m_iCorner[2], e4));
	Node->m_ChildNode.push_back(CreatorNode(Node, e2, e3, e4, Node->m_iCorner[3]));

	return true;

}

SNode* SQuadTree::CreatorNode(SNode* Node, float tl, float tr, float bl, float br)
{
	SNode* pNode = new SNode;
	pNode->m_iCorner[TL] = tl;
	pNode->m_iCorner[TR] = tr;
	pNode->m_iCorner[BL] = bl;
	pNode->m_iCorner[BR] = br;
	pNode->m_Depth = Node->m_Depth + 1;

	ldiv_t divValue = ldiv(tl, m_Map->m_iNumRows);
	pNode->m_dwPositionIndex[0] = divValue.rem / (tr - tl);
	pNode->m_dwPositionIndex[1] = divValue.quot / (tr - tl);

	m_LevelLIst[pNode->m_Depth].push_back(pNode);
	CreatorIndexBuffer(Node, m_Map->m_iNumCols, m_Map->m_iNumRows);
	Division(pNode);

	return pNode;
}

void SQuadTree::CreatorIndexBuffer(SNode* Node, float row, float col)
{
	DWORD dwTL = Node->m_iCorner[0];
	DWORD dwTR = Node->m_iCorner[1];
	DWORD dwBL = Node->m_iCorner[2];
	DWORD dwBR = Node->m_iCorner[3];

	DWORD dwSize = (dwTR - dwTL)*(dwTR - dwTL) * 2 * 3;
	Node->m_IndexList.resize(dwSize);

	int dwCurentIndex = 0;

	DWORD dwStartRow = dwTL / col;
	DWORD dwEndRow = dwBL / col;

	DWORD dwStartCol = dwTL % (DWORD)col;
	DWORD dwEndCol = dwTR % (DWORD)col;

	for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol++)
		{
			//0	1    4   
			//2	   3 5
			DWORD dwNextRow = dwRow + 1;
			DWORD dwNextCol = dwCol + 1;
			Node->m_IndexList[dwCurentIndex++] = dwRow * col + dwCol;
			Node->m_IndexList[dwCurentIndex++] = dwRow * col + dwNextCol;
			Node->m_IndexList[dwCurentIndex++] = dwNextRow * col + dwCol;
			Node->m_IndexList[dwCurentIndex++] = dwNextRow * col + dwCol;
			Node->m_IndexList[dwCurentIndex++] = dwRow * col + dwNextCol;
			Node->m_IndexList[dwCurentIndex++] = dwNextRow * col + dwNextCol;
		}
	}
	Node->m_pIndexBuffer =CreateIndexBuffer(g_pd3dDevice,
		&Node->m_IndexList.at(0),
		Node->m_IndexList.size(),
		sizeof(DWORD));
}

bool SQuadTree::Render(ID3D11DeviceContext*	pd3dContext)
{
	UINT iStride = sizeof(PNCT_VERTEX);
	UINT iOffset = 0;
	m_Map->PreRender(pd3dContext);
	//	&m_pTexture->m_pTextureSRV);

	for (int i = 0; i < m_LODDrawLIst.size(); i++)
	{
		pd3dContext->IASetIndexBuffer(m_LODDrawLIst[i]->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		pd3dContext->DrawIndexed(m_LODDrawLIst[i]->m_IndexList.size(), 0, 0);
	}
	
	return false;
}

Vector2 SQuadTree::GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR)
{
	DWORD dwStartRow = dwTL / m_Map->m_iNumCols;
	DWORD dwEndRow = dwBL / m_Map->m_iNumCols;

	DWORD dwStartCol = dwTL % m_Map->m_iNumCols;
	DWORD dwEndCol = dwTR % m_Map->m_iNumCols;

	Vector2 vHeight;
	vHeight.x = -999999.0f;
	vHeight.y = 999999.0f;
	for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol++)
		{
			if (m_Map->m_VertexList[dwRow *  m_Map->m_iNumCols + dwCol].p.y > vHeight.x)
			{
				vHeight.x = m_Map->m_VertexList[dwRow *  m_Map->m_iNumCols + dwCol].p.y;
			}
			if (m_Map->m_VertexList[dwRow *  m_Map->m_iNumCols + dwCol].p.y < vHeight.y)
			{
				vHeight.y = m_Map->m_VertexList[dwRow *  m_Map->m_iNumCols + dwCol].p.y;
			}
		}
	}
	return vHeight;
}

bool SQuadTree::Release()
{
	//if(m_Map != nullptr)m_Map->Release();
	m_DrawLIst.clear();
	return false;
}

SQuadTree::SQuadTree()
{
}

SQuadTree::~SQuadTree()
{
	
}

void SQuadTree::BoungigBox(SNode * Node)
{
	//Node->m_VertexList[(int)Node->m_iCorner[0]];
	//Node->m_VertexList[(int)Node->m_iCorner[1]];
	//Node->m_VertexList[(int)Node->m_iCorner[2]];
	//Node->m_VertexList[(int)Node->m_iCorner[3]];

	Vector2 vHeight = GetHeightFromNode(Node->m_iCorner[0], Node->m_iCorner[1],
		Node->m_iCorner[2], Node->m_iCorner[3]);

	Node->m_Box.vMax = m_Map->m_VertexList[(int)Node->m_iCorner[1]].p;
	Node->m_Box.vMin = m_Map->m_VertexList[(int)Node->m_iCorner[2]].p;
	Node->m_Box.vMax.y = vHeight.x;
	Node->m_Box.vMin.y = vHeight.y;
		  
	Node->m_Box.vCenter = (Node->m_Box.vMax + Node->m_Box.vMin) / 2.0f;
	Node->m_Box.fExtent[0] = Node->m_Box.vMax.x - Node->m_Box.vCenter.x;
	Node->m_Box.fExtent[1] = Node->m_Box.vMax.y - Node->m_Box.vCenter.y;
	Node->m_Box.fExtent[2] = Node->m_Box.vMax.z - Node->m_Box.vCenter.z;

}

void	SQuadTree::DrawCheck(SNode* Node)
{
	//if (Node->m_LeafNode)
	//{
	//	m_DrawLIst.push_back(Node);
	//	return;
	//}	
	//for (int iNode = 0; iNode < Node->m_ChildNode.size(); iNode++)
	//{
	//	DrawCheck(Node->m_ChildNode[iNode]);
	//}

	S_POSITION a = m_mainCamera->CheckPoitionOBBInPlane(&Node->m_Box);
	if (Node->m_LeafNode &&  a != P_BACK)
	{
		GetLodSubIndex(Node);
		m_DrawLIst.push_back(Node);
		return;
	}
	if (a == P_FRONT)
	{
		GetLodSubIndex(Node);
		m_DrawLIst.push_back(Node);
		return;
	}

	for (int iNode = 0; iNode < Node->m_ChildNode.size(); iNode++)
	{
		DrawCheck(Node->m_ChildNode[iNode]);
	}
}

bool	SQuadTree::Frame()
{
	m_DrawLIst.clear();
	DrawCheck(m_RootNode);
	GetDrowList();

	Face = 0;
	for (int i = 0; i < m_LODDrawLIst.size(); i++)
	{
		GetLodType(m_LODDrawLIst[i]);
		Face += UpdateIndexList(m_LODDrawLIst[i],
			Face * 3,
			m_LODDrawLIst[i]->m_LODLevel);
	}
	return true;
}


///////////////
// 이웃 노드 추가 
void SQuadTree::SetNeighvorNode(SNode* Node)
{
	SetNeighvor(Node);
	for (int i = 0; i < Node->m_ChildNode.size(); i++)
	{
		SetNeighvorNode(Node->m_ChildNode[i]);
	}
}
void SQuadTree::SetNeighvor(SNode* Node)
{
	Node->m_NeghborList.resize(4);

	DWORD dwNumPatchCount = (DWORD)pow(2.0f, (float)Node->m_Depth);

	DWORD dwNeighborCol, dwNeighborRow;
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
// 리프 노드만 저장
void	SQuadTree::GetDrowList()
{
	m_LODDrawLIst.clear();
	
	for (int i = 0; i < m_DrawLIst.size(); i++)
	{
		AddDrowList(m_DrawLIst[i]);
	}
} 
void SQuadTree::AddDrowList(SNode* Node)
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
DWORD SQuadTree::GetLodSubIndex(SNode* Node)
{
	int iNumPatchIndex = m_iNumCell;
	float fRatio = GetExpansionRatio(Node->m_Box.vCenter);
	DWORD dwCurentRatio = (DWORD)(fRatio * m_iPatchLodCount);
	/*DWORD dwMaxRatio = (DWORD)(fRatio * m_iPatchLodCount + 0.5f);
	DWORD dwMinRatio = (DWORD)(fRatio * m_iPatchLodCount - 0.5f);
	//if (Node->m_LODLevel < dwCurentRatio)
	//{
	//	if (Node->m_LODLevel < dwMinRatio)		
	//	{
	//		Node->m_LODLevel = dwCurentRatio;
	//	}
	//}
	//else if (Node->m_LODLevel > dwCurentRatio)
	//{
	//	if (Node->m_LODLevel > dwMaxRatio)		
	//	{
	//		Node->m_LODLevel = dwCurentRatio;
	//	}
	//}*/
	Node->m_LODLevel = dwCurentRatio;
	if ((DWORD)m_iPatchLodCount < Node->m_LODLevel)
	{
		Node->m_LODLevel -= 1;
	}

	return Node->m_LODLevel;
}
void  SQuadTree::SetLOD(int iNumDepth)
{
	m_iNumCell = (int)((m_Map->m_iNumCellRows) / pow(2.0f, (float)iNumDepth));
	m_iPatchLodCount = (int)((log((float)m_iNumCell) / log(2.0f)));
}
float SQuadTree::GetExpansionRatio(Vector3 vCenter)
{
	
	float fDistance = (m_mainCamera->m_vCameraPos - vCenter).Length();
	if (fDistance > m_mainCamera->m_fFarPlane)
	{
		return 1.0f;
	}
	float fRatio = fDistance / m_mainCamera->m_fFarPlane;
	return fRatio;
}

DWORD SQuadTree::GetLodType(SNode* pNode)
{
	if (pNode->m_NeghborList.size() <= 0) return 0;
	DWORD dwType = 0;
	if (pNode->m_NeghborList[0] && pNode->m_NeghborList[0]->m_LODLevel < pNode->m_LODLevel) dwType += 1;
	if (pNode->m_NeghborList[1] && pNode->m_NeghborList[1]->m_LODLevel < pNode->m_LODLevel) dwType += 4;
	if (pNode->m_NeghborList[2] && pNode->m_NeghborList[2]->m_LODLevel < pNode->m_LODLevel) dwType += 8;
	if (pNode->m_NeghborList[3] && pNode->m_NeghborList[3]->m_LODLevel < pNode->m_LODLevel) dwType += 2;

	pNode->m_LODType = dwType;
	return dwType;
}
int SQuadTree::UpdateIndexList(SNode* pNode, DWORD dwCurentIndex, DWORD dwNumLevel)
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
int  SQuadTree::SetLodIndexBuffer(SNode* pNode,	DWORD& dwCurentIndex,DWORD dwA, DWORD dwB, DWORD dwC,DWORD dwType)

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