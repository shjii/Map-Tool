#include "SQuadTree.h"
bool	SQuadTree::Build(SMap* m_Map)
{
	this->m_Map = m_Map;

	m_RootNode = BuildNode(m_Map);
	
	BoungigBox(m_RootNode);

	if (Division(m_RootNode))
	{
		
	}
	return true;
}

SNode * SQuadTree::BuildNode(SMap * m_Map)
{
	SNode* sNode = new SNode;
	sNode->m_Depth = 0;
	sNode->m_iCorner[TL] = 0;
	sNode->m_iCorner[TR] = m_Map->m_iNumCols - 1 ;
	sNode->m_iCorner[BL] = m_Map->m_iNumRows * m_Map->m_iNumRows - (m_Map->m_iNumRows - 1);
	sNode->m_iCorner[BR] = m_Map->m_iNumCols * m_Map->m_iNumRows;
	sNode->m_IndexList = m_Map->m_IndexList;
	sNode->m_VertexList = m_Map->m_VertexList;
	sNode->m_pIndexBuffer = m_Map->m_pIndexBuffer;
	sNode->m_pVertexBuffer = m_Map->m_pVertexBuffer;
	return sNode;
}
bool	SQuadTree::Division(SNode* Node)
{
	if (Node == NULL) return false;

	

	if (Node->m_Depth >= m_MaxDepth)
	{
		
		Node->m_LeafNode = true;
		return false;
	}

	float centerCol = (Node->m_iCorner[TR] - Node->m_iCorner[TL]) / 2;
	float centerRow = (Node->m_iCorner[BL] - Node->m_iCorner[TL]) / 2;

	// ¹ØÀ¸·Î row ¿·À¸·Î col
	Node->m_ChildNode.push_back(CreatorNode(Node, Node->m_iCorner[TL], Node->m_iCorner[TL] + centerCol, Node->m_iCorner[TL] + centerRow, Node->m_iCorner[TL] + centerRow + centerCol));
	Node->m_ChildNode.push_back(CreatorNode(Node, Node->m_iCorner[TL] + centerCol, Node->m_iCorner[TR], Node->m_iCorner[TL] + centerRow + centerCol, Node->m_iCorner[TR] + centerRow));
	Node->m_ChildNode.push_back(CreatorNode(Node, Node->m_iCorner[TL] + centerRow, Node->m_iCorner[TL] + centerRow + centerCol, Node->m_iCorner[BL], Node->m_iCorner[BL] + centerCol));
	Node->m_ChildNode.push_back(CreatorNode(Node, Node->m_iCorner[TL] + centerRow + centerCol, Node->m_iCorner[TR] + centerRow, Node->m_iCorner[BL] + centerCol, Node->m_iCorner[BR]));

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
	pd3dContext->IASetIndexBuffer(m_RootNode->m_ChildNode[0]->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pd3dContext->DrawIndexed(m_RootNode->m_ChildNode[0]->m_IndexList.size(), 0, 0);
	
	//pd3dContext->Draw(m_VertexList.size(), 0);
	
	return false;
}

void SQuadTree::BoungigBox(SNode * Node)
{
	//Node->m_VertexList[(int)Node->m_iCorner[0]];
	//Node->m_VertexList[(int)Node->m_iCorner[1]];
	//Node->m_VertexList[(int)Node->m_iCorner[2]];
	//Node->m_VertexList[(int)Node->m_iCorner[3]];

	Node->m_Box.vMax = m_Map->m_VertexList[(int)Node->m_iCorner[0]].p;
	Node->m_Box.vMin = m_Map->m_VertexList[(int)Node->m_iCorner[2]].p;
	//Node->m_Box.vMax.y = vHeight.x;
	//Node->m_Box.vMin.y = vHeight.y;
		  
	Node->m_Box.vCenter = (Node->m_Box.vMax + Node->m_Box.vMin) / 2.0f;
	Node->m_Box.fExtent[0] = Node->m_Box.vMax.x - Node->m_Box.vCenter.x;
	Node->m_Box.fExtent[1] = Node->m_Box.vMax.y - Node->m_Box.vCenter.y;
	Node->m_Box.fExtent[2] = Node->m_Box.vMax.z - Node->m_Box.vCenter.z;

}
