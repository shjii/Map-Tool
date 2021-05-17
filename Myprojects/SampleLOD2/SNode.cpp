#include "SNode.h"

SNode::SNode()
{
	m_dwLodType = 0;
	m_dwDepth = 0;
	m_isLeaf = FALSE;
	S_BOX.vCenter = Vector3(0.0f, 0.0f, 0.0f);
	S_BOX.vMax = Vector3(1.0f, 1.0f, 1.0f);
	S_BOX.vMin = Vector3(-1.0f, -1.0f, -1.0f);
	S_BOX.vAxis[0] = Vector3(1.0f, 0.0f, 0.0f);
	S_BOX.vAxis[1] = Vector3(0.0f, 1.0f, 0.0f);
	S_BOX.vAxis[2] = Vector3(0.0f, 0.0f, 1.0f);
	S_BOX.fExtent[0] = 1;
	S_BOX.fExtent[1] = 1;
	S_BOX.fExtent[2] = 1;


	iNumPolygon = 0;
	iNumMtrl = 0;
	m_pSplitter = NULL;
}
SNode::~SNode()
{
	// 노드의 코너 정점 인덱스 및 위치
	if (!m_CornerList.empty()) m_CornerList.clear();
	if (!m_CornerIndex.empty()) m_CornerIndex.clear();	// 추가

	for (int i = 0; i < m_ChildList.size(); i++)
	{
		SAFE_DEL(m_ChildList[i]);
	}
	//////////// bsp 
	// 노드의 분할면 및 리프노드의 폴리곤 리스트 소멸
	/*TPolygon *pDelSplitter = m_pSplitter;
	while (pDelSplitter)
	{
		TPolygon *pPolyDel = pDelSplitter;
		pDelSplitter = pPolyDel->pNext;
		SAFE_DEL(pPolyDel);
	}*/

	/*if (m_ChildList.size() > 0)
	{
		for (int ibuffer = 0; ibuffer < iNumMtrl; ibuffer++)
		{
			SAFE_DEL(m_pVBList[ibuffer]);
			SAFE_DEL(m_pIBList[ibuffer]);
		}
	}*/

	if (!m_pVBList.empty()) m_pVBList.clear();
	if (!m_pIBList.empty()) m_pIBList.clear();
	if (!m_MtrlList.empty()) m_MtrlList.clear();
}
