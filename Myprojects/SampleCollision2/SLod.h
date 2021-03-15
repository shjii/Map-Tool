#pragma once
#include "SNode.h"
#include "SMap.h"
#include "SCamera.h"
class SLod
{
public:
	SNode*					m_RootNode;
	vector<SNode*>			m_LODDrawLIst;
	vector<vector<SNode*>>	m_LevelLIst;
	int						m_iNumCell;
	int						m_iPatchLodCount;
	int						Face;
	vector<DWORD>			m_IndexList;
	SCamera*				m_mainCamera;
	vector<SNode*>			m_DrawLIst;
	SMap*					m_Map;
public:
	void	GetDrowList();
	void	AddDrowList(SNode* Node);
	void SetNeighvorNode(SNode* Node);
	void SetNeighvor(SNode* Node);
	DWORD GetLodSubIndex(SNode* Node);
	void  SetLOD(int iNumDepth);
	float GetExpansionRatio(Vector3 vCenter);
	DWORD GetLodType(SNode* pNode);
	int	  UpdateIndexList(SNode* pNode, DWORD dwCurentIndex, DWORD dwNumLevel);
	int   SetLodIndexBuffer(SNode* pNode, DWORD& dwCurentIndex, DWORD dwA, DWORD dwB, DWORD dwC, DWORD dwType);
};

