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
	for (int i = 1; i < m_LevelLIst.size(); i++)
	{
		m_LevelLIst[i].resize(m_LevelLIst[i-1].size() * 4);
	}
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
	sNode->m_iCorner[TR] = m_Map->m_iNumCols - 1;
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

	// ¹ØÀ¸·Î row ¿·À¸·Î col
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

	DWORD dwIndex =
		Node->m_ChildNode[0]->m_dwPositionIndex[1] * pow(2.0f, (float)Node->m_ChildNode[0]->m_Depth) + Node->m_ChildNode[0]->m_dwPositionIndex[0];
	DWORD dwValue = Node->m_ChildNode[0]->m_Depth;
	m_LevelLIst[dwValue][dwIndex] =
		Node->m_ChildNode[0];
	Node->m_ChildNode.push_back(CreatorNode(Node, e0, Node->m_iCorner[1], e2, e3));

	 dwIndex =
		Node->m_ChildNode[1]->m_dwPositionIndex[1] * pow(2.0f, (float)Node->m_ChildNode[1]->m_Depth) + Node->m_ChildNode[1]->m_dwPositionIndex[0];
	 dwValue = Node->m_ChildNode[1]->m_Depth;
	m_LevelLIst[dwValue][dwIndex] =
		Node->m_ChildNode[1];
	Node->m_ChildNode.push_back(CreatorNode(Node, e1, e2, Node->m_iCorner[2], e4));

	 dwIndex =
		Node->m_ChildNode[2]->m_dwPositionIndex[1] * pow(2.0f, (float)Node->m_ChildNode[2]->m_Depth) + Node->m_ChildNode[2]->m_dwPositionIndex[0];
	 dwValue = Node->m_ChildNode[2]->m_Depth;
	m_LevelLIst[dwValue][dwIndex] =
		Node->m_ChildNode[2];
	Node->m_ChildNode.push_back(CreatorNode(Node, e2, e3, e4, Node->m_iCorner[3]));

	 dwIndex =
		Node->m_ChildNode[3]->m_dwPositionIndex[1] * pow(2.0f, (float)Node->m_ChildNode[3]->m_Depth) + Node->m_ChildNode[3]->m_dwPositionIndex[0];
	 dwValue = Node->m_ChildNode[3]->m_Depth;
	m_LevelLIst[dwValue][dwIndex] =
		Node->m_ChildNode[3];

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

	//DWORD dwIndex =
	//	Node->m_dwPositionIndex[1] * pow(2.0f, (float)Node->m_Depth + 1) + pNode->m_dwPositionIndex[0];

	//m_LevelLIst[pNode->m_Depth][dwIndex] = Node;

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
	Node->m_pIndexBuffer = CreateIndexBuffer(g_pd3dDevice,
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
	delete m_RootNode;
	m_DrawLIst.clear();
	for (int i = 0; i < m_LevelLIst.size(); i++)
	{
		for (int j = 0; j < m_LevelLIst[i].size(); j++)
		{
			if (m_LevelLIst[j][i])
			{
				delete m_LevelLIst[j][i];
				m_LevelLIst[j][i] = NULL;
			}
		}
	}
	m_LevelLIst.clear();
	return false;
}

void SQuadTree::GetUpdata(SCamera * C)
{
	m_mainCamera = C;
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

	Node->m_Sphere.vCenter = Node->m_Box.vCenter;
	Node->m_Sphere.fRadius = (Node->m_Box.vMax - Node->m_Box.vCenter).Length();

}

void	SQuadTree::DrawCheck(SNode* Node)
{
	int b = GetLodSubIndex(Node);
	if (b != 1)
	{
		b = 0;
	}
	S_POSITION a = m_mainCamera->CheckPoitionOBBInPlane(&Node->m_Box);
	if (Node->m_LeafNode &&  a != P_BACK)
	{
		m_DrawLIst.push_back(Node);
		return;
	}
	if (a == P_FRONT)
	{

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
	m_IndexList.clear();
	m_IndexList.resize(m_Map->m_iNumCellCols * m_Map->m_iNumCellRows * 2 * 2 * 3);
	for (int i = 0; i < m_LODDrawLIst.size(); i++)
	{
		GetLodType(m_LODDrawLIst[i]);
		Face += UpdateIndexList(m_LODDrawLIst[i],
			Face * 3,
			m_LODDrawLIst[i]->m_LODLevel);
	}
	return true;
}