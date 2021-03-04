#include "SMapRender.h"

bool		SMapRender::Build(SMap* pMap, DWORD dwWidth, DWORD dwHeight)
{
	m_dwHeight = dwHeight;
	m_dwWidth = dwWidth;
	m_pMap = pMap;
	m_iNumFace = (dwWidth - 1)*(dwHeight - 1) * 2;
	m_bUsedIndexList = true;

	m_pRooSNode = CreateNode(NULL, 0, dwWidth - 1, dwWidth * (dwHeight - 1), dwWidth * dwHeight - 1);

	InitLevelOrder(m_pRooSNode, m_iMaxDepthLimit);

	m_pIndexBuffer.Attach(CreateIndexBuffer(g_pd3dDevice, NULL, m_iNumFace * 3, sizeof(DWORD)));

	if (BuildTree(m_pRooSNode))
	{
		for (int iNode = 0; iNode < m_pRooSNode->m_ChildList.size(); iNode++)
		{
			SetNeighborNode(m_pRooSNode->m_ChildList[iNode]);
		}

		int iNumTileRender = pow(4.0f, m_iMaxDepth);
		// <><<>><><><
		m_iNumCell = (dwWidth - 1) / pow(2.0f, (float)m_iMaxDepth);
		m_IndexList.resize(m_iNumCell * m_iNumCell * 2 * 3);

		int iIndex = 0;
		for (DWORD dwRow = 0; dwRow < m_iNumCell; dwRow++)
		{
			for (DWORD dwCol = 0; dwCol < m_iNumCell; dwCol++)
			{
				//0	1    4   
				//2	   3 5
				DWORD dwNextRow = dwRow + 1;
				DWORD dwNextCol = dwCol + 1;
				m_IndexList[iIndex++] = (dwRow * (m_iNumCell + 1) + dwCol);
				m_IndexList[iIndex++] = (dwRow * (m_iNumCell + 1) + dwNextCol);
				m_IndexList[iIndex++] = (dwNextRow * (m_iNumCell + 1) + dwCol);
				m_IndexList[iIndex++] = (dwNextRow * (m_iNumCell + 1) + dwCol);
				m_IndexList[iIndex++] = (dwRow * (m_iNumCell + 1) + dwNextCol);
				m_IndexList[iIndex++] = (dwNextRow * (m_iNumCell + 1) + dwNextCol);
			}
		}
		m_pTileIndexBuffer.Attach(CreateIndexBuffer(g_pd3dDevice, &m_IndexList.at(0), m_IndexList.size(), sizeof(DWORD)));
		return true;
	}
	return false;
}
SNode*		SMapRender::CreateNode(SNode* pParentNode, DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR)
{
	SNode* pNode = new SNode;
	assert(pNode);

	pNode->m_isLeaf = FALSE;
	pNode->m_CornerIndex.push_back(dwTL);
	pNode->m_CornerIndex.push_back(dwTR);
	pNode->m_CornerIndex.push_back(dwBL);
	pNode->m_CornerIndex.push_back(dwBR);
	ComputeBoungigBox(pNode);

	//���� ������ �� �� / ������
	ldiv_t divValue = ldiv(dwTL, m_dwWidth);
	pNode->m_dwPositionIndex[0] = divValue.rem / (dwTR - dwTL); // ������-> X
	pNode->m_dwPositionIndex[1] = divValue.quot / (dwTR - dwTL);//�� -> Y

	if (pParentNode)
	{
		pNode->m_dwDepth = pParentNode->m_dwDepth + 1;
		if (m_iMaxDepth < pNode->m_dwDepth)
		{
			m_iMaxDepth = pNode->m_dwDepth;
		}
	}
	return pNode;
}
Vector2		SMapRender::GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR)
{
	assert(m_pMap);

	DWORD dwStartRow = dwTL / m_dwWidth;
	DWORD dwEndRow = dwBL / m_dwWidth;
	DWORD dwStartCol = dwTL % m_dwWidth;
	DWORD dwEndCol = dwTR % m_dwWidth;

	Vector2 vHeight;
	vHeight.x = -9999999.0f;
	vHeight.y = 9999999.0f;
	for (DWORD dwRow = 0; dwRow < dwEndRow; dwRow++)
	{
		for (DWORD dwcol = 0; dwcol < dwEndCol; dwcol++)
		{
			if (m_pMap->m_VertexList[dwRow * m_dwWidth + dwcol].p.y > vHeight.x)
			{
				vHeight.x = m_pMap->m_VertexList[dwRow * m_dwWidth + dwcol].p.y;
			}
			if (m_pMap->m_VertexList[dwRow * m_dwWidth + dwcol].p.y < vHeight.y)
			{
				vHeight.y = m_pMap->m_VertexList[dwRow * m_dwWidth + dwcol].p.y;
			}
		}
	}
	return vHeight;
}
void		SMapRender::ComputeBoungigBox(SNode* pNode)
{
	// ���ؽ� ����Ʈ�� y�� ���� ������ x ������ y ...??
	Vector2 vHeight = GetHeightFromNode(pNode->m_CornerIndex[0], pNode->m_CornerIndex[1], pNode->m_CornerIndex[2], pNode->m_CornerIndex[3]);

	pNode->m_CornerList.push_back(m_pMap->m_VertexList[pNode->m_CornerIndex[0]].p);
	pNode->m_CornerList.push_back(m_pMap->m_VertexList[pNode->m_CornerIndex[1]].p);
	pNode->m_CornerList.push_back(m_pMap->m_VertexList[pNode->m_CornerIndex[2]].p);
	pNode->m_CornerList.push_back(m_pMap->m_VertexList[pNode->m_CornerIndex[3]].p);

	pNode->m_sBox.vMax = m_pMap->m_VertexList[pNode->m_CornerIndex[1]].p;
	pNode->m_sBox.vMin = m_pMap->m_VertexList[pNode->m_CornerIndex[2]].p;
	pNode->m_sBox.vMax.y = vHeight.x;
	pNode->m_sBox.vMin.y = vHeight.y;

	pNode->m_sBox.vCenter = (pNode->m_sBox.vMax + pNode->m_sBox.vMin) / 2.0f;
	pNode->m_sBox.fExtent[0] = pNode->m_sBox.vMax.x - pNode->m_sBox.vCenter.x;
	pNode->m_sBox.fExtent[1] = pNode->m_sBox.vMax.y - pNode->m_sBox.vCenter.y;
	pNode->m_sBox.fExtent[2] = pNode->m_sBox.vMax.z - pNode->m_sBox.vCenter.z;
}
void		SMapRender::UpdateBoundingBox(SNode* pNode)
{
	ComputeBoungigBox(pNode);
	for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
	{
		UpdateBoundingBox(pNode->m_ChildList[iNode]);
	}
}
bool		SMapRender::SubDivide(SNode* pNode)
{
	if (pNode == NULL) return false;
	
	DWORD dwOffsetX = (pNode->m_CornerIndex[tTR] - pNode->m_CornerIndex[tTL]);
	DWORD dwOffsetZ = ((pNode->m_CornerIndex[tBL] - pNode->m_CornerIndex[tTL]) / m_dwWidth);
	DWORD dwWidthSplit = CheckSize(dwOffsetX);
	DWORD dwHeightplit = CheckSize(dwOffsetZ);

	if (m_iMaxDepthLimit <= pNode->m_dwDepth)
	{
		pNode->m_isLeaf = TRUE;
		CreateVertexList(pNode);
		CreateIndexList(pNode);
		return false;
	}
	if (dwWidthSplit < m_iMinSplitDistance || dwHeightplit < m_iMinSplitDistance)
	{
		pNode->m_isLeaf = TRUE;
		CreateVertexList(pNode);
		CreateIndexList(pNode);
		return false;
	}
	float fWidthSplit = (pNode->m_CornerList[tTR].x - pNode->m_CornerList[tTL].x) / 2;
	float fHeightSplit = (pNode->m_CornerList[tTL].z - pNode->m_CornerList[tBL].z) / 2;
	// �ڽ� ��尡 ������ ����ũ�� ���� �۴ٸ� ���̻� �������� �ʴ´�.
	if (fWidthSplit < m_fMinDivideSize || fHeightSplit < m_fMinDivideSize)
	{
		pNode->m_isLeaf = TRUE;
		CreateVertexList(pNode);
		CreateIndexList(pNode);
		return false;
	}

	// ���� = �����ڳ� + ���ø�ũ��
	// ���ϱ� = ���� + ( ���ø� ũ�� * ��ü�� ����ũ�� )
	DWORD dwSplitCenter = (pNode->m_CornerIndex[tTL] + dwWidthSplit) + (dwHeightplit*m_dwWidth);

	DWORD dwEdgeCenter[4];
	dwEdgeCenter[0] = pNode->m_CornerIndex[tTL] + dwWidthSplit;
	dwEdgeCenter[1] = pNode->m_CornerIndex[tTR] + dwHeightplit * m_dwWidth;
	dwEdgeCenter[2] = pNode->m_CornerIndex[tBL] + dwWidthSplit;
	dwEdgeCenter[3] = pNode->m_CornerIndex[tTL] + dwHeightplit * m_dwWidth;

	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_CornerIndex[tTL],
		dwEdgeCenter[0],
		dwEdgeCenter[3],
		dwSplitCenter));

	pNode->m_ChildList.push_back(CreateNode(pNode, dwEdgeCenter[0],
		pNode->m_CornerIndex[tTR],
		dwSplitCenter,
		dwEdgeCenter[1]));

	pNode->m_ChildList.push_back(CreateNode(pNode, dwEdgeCenter[3],
		dwSplitCenter,
		pNode->m_CornerIndex[tBL],
		dwEdgeCenter[2]));

	pNode->m_ChildList.push_back(CreateNode(pNode, dwSplitCenter,
		dwEdgeCenter[1],
		dwEdgeCenter[2],
		pNode->m_CornerIndex[tBR]));
	return true;
}
bool		SMapRender::Release()
{
	return SQuadTree::Release();
}
bool		SMapRender::Render(ID3D11DeviceContext*    pContext)
{
	//m_DrawPatchNodeList ����
	m_DrawPatchNodeList.clear();
	// ī�޶� ���̴� ��帮��Ʈ���� �������(��ġ����)�� ��带 ��´�.
	GetDrawPatchNode();

	m_pMap->PreRender(pContext);
	UINT stride = m_pMap->m_VertexList.size();
	UINT offset = 0;
	pContext->UpdateSubresource(m_pMap->m_pConstantBuffer, 0, NULL, &m_pMap->m_cbData, 0, 0);

	if (m_bSharedVertexBuffer)
	{
		size_t dstibOffset = 0;
		size_t ibOffset = 0;
		// ��� -1-
		pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		for (int iNode = 0; iNode < m_DrawPatchNodeList.size(); iNode++)
		{
			SNode* pNode = m_DrawPatchNodeList[iNode];
			// ��� -1-
			//pContext->DrawIndexed(pNode->m_IndexList.size(), pNode->m_iBeginIB, 0);

			//���-2- �ǽð� ������Ʈ ������
	/*		pNode->m_BoxIB.left = dstibOffset;
			pNode->m_BoxIB.right = dstibOffset + pNode->m_IndexList.size()*sizeof(DWORD);
			pNode->m_BoxIB.top = 0;		pNode->m_BoxIB.bottom = 1;
			pNode->m_BoxIB.front = 0;	pNode->m_BoxIB.back = 1;

			g_pImmediateContext->UpdateSubresource(m_pMap->m_dxobj.g_pIndexBuffer.Get(), 0,
				&pNode->m_BoxIB, (void*)&pNode->m_IndexList.at(0), 0, 0);

			pNode->m_iBeginIB = ibOffset;
			ibOffset += pNode->m_IndexList.size();
			dstibOffset = pNode->m_BoxIB.right;*/

			// ��� -3-���� ��� �ε��� ���۸� ����Ͽ� ���� ������
			pContext->IASetIndexBuffer(pNode->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			pContext->DrawIndexed(pNode->m_IndexList.size(), 0, 0);
		}
		//���-2-
		//pContext->DrawIndexed(ibOffset, 0, 0);
	}
	else
	{
		pContext->IASetIndexBuffer(m_pTileIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		for (int iNode = 0; iNode < m_DrawPatchNodeList.size(); iNode++)
		{
			SNode* pNode = m_DrawPatchNodeList[iNode];
			pContext->IASetVertexBuffers(0, 1, pNode->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
			pContext->DrawIndexed(m_IndexList.size(), 0, 0);
		}
	}
	return true;
}
bool		SMapRender::CreateVertexList(SNode* pNode)
{
	DWORD dwTL = pNode->m_CornerIndex[0];
	DWORD dwTR = pNode->m_CornerIndex[1];
	DWORD dwBL = pNode->m_CornerIndex[2];
	DWORD dwBR = pNode->m_CornerIndex[3];

	DWORD dwSize = ((dwTR - dwTL) + 1)*((dwTR - dwTL) + 1);
	pNode->m_VertexList.resize(dwSize);

	DWORD dwStartRow = dwTL / m_dwWidth;
	DWORD dwEndRow = dwBL / m_dwWidth;

	DWORD dwStartCol = dwTL % m_dwWidth;
	DWORD dwEndCol = dwTR % m_dwWidth;

	DWORD iVertexIndex = 0;
	for (DWORD dwRow = dwStartRow; dwRow <= dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol <= dwEndCol; dwCol++)
		{
			pNode->m_VertexList[iVertexIndex++] = m_pMap->m_VertexList[dwRow*m_dwWidth + dwCol];
		}
	}

	pNode->m_pVertexBuffer.Attach(CreateVertexBuffer(g_pd3dDevice,
		&pNode->m_VertexList.at(0),
		pNode->m_VertexList.size(),
		sizeof(PNCT_VERTEX)));
	return true;
}
int			SMapRender::CreateIndexList(SNode* pNode)
{
	vector<DWORD> Indexlist;
	DWORD dwTL = pNode->m_CornerIndex[0];
	DWORD dwTR = pNode->m_CornerIndex[1];
	DWORD dwBL = pNode->m_CornerIndex[2];
	DWORD dwBR = pNode->m_CornerIndex[3];

	DWORD dwSize = (dwTR - dwTL)*(dwTR - dwTL) * 2 * 3;
	pNode->m_IndexList.resize(dwSize);

	int dwCurentIndex = 0;

	DWORD dwStartRow = dwTL / m_dwWidth;
	DWORD dwEndRow = dwBL / m_dwWidth;

	DWORD dwStartCol = dwTL % m_dwWidth;
	DWORD dwEndCol = dwTR % m_dwWidth;

	for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol++)
		{
			//0	1    4   
			//2	   3 5
			DWORD dwNextRow = dwRow + 1;
			DWORD dwNextCol = dwCol + 1;
			pNode->m_IndexList[dwCurentIndex++] = dwRow * m_dwWidth + dwCol;
			pNode->m_IndexList[dwCurentIndex++] = dwRow * m_dwWidth + dwNextCol;
			pNode->m_IndexList[dwCurentIndex++] = dwNextRow * m_dwWidth + dwCol;
			pNode->m_IndexList[dwCurentIndex++] = dwNextRow * m_dwWidth + dwCol;
			pNode->m_IndexList[dwCurentIndex++] = dwRow * m_dwWidth + dwNextCol;
			pNode->m_IndexList[dwCurentIndex++] = dwNextRow * m_dwWidth + dwNextCol;
		}
	}
	pNode->m_pIndexBuffer.Attach(CreateIndexBuffer(g_pd3dDevice,
		&pNode->m_IndexList.at(0),
		pNode->m_IndexList.size(),
		sizeof(DWORD)));

	pNode->m_BoxIB.left = m_dstibOffset;
	pNode->m_BoxIB.right = m_dstibOffset + pNode->m_IndexList.size() * sizeof(DWORD);
	pNode->m_BoxIB.top = 0;		pNode->m_BoxIB.bottom = 1;
	pNode->m_BoxIB.front = 0;	pNode->m_BoxIB.back = 1;

	g_pImmediateContext->UpdateSubresource(m_pIndexBuffer.Get(), 0, &pNode->m_BoxIB, (void*)&pNode->m_IndexList.at(0), 0, 0);

	pNode->m_iBeginIB = m_ibOffset;
	m_ibOffset += pNode->m_IndexList.size();
	m_dstibOffset = pNode->m_BoxIB.right;

	return Indexlist.size() / 3;
}

SMapRender::SMapRender(void)
{
	m_dwWidth = 0;
	m_dwHeight = 0;
	m_iNumFace = 0;
	m_iMinSplitDistance = 2;
	m_bSharedVertexBuffer = true;
	m_dstibOffset = 0;
	m_ibOffset = 0;
	SAFE_ZERO(m_pMap);
}
SMapRender::~SMapRender(void)
{
}