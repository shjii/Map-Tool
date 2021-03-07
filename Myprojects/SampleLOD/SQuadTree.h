#pragma once
#include "SNode.h"
#include "SMap.h"
#include "SCamera.h"
enum CornerPosition
{
	TL = 0,
	TR,
	BL,
	BR,
};
class SQuadTree 
{
public:
	SMap*			m_Map;
	SNode*			m_RootNode;
	int				m_MaxDepth;
	SCamera*		m_mainCamera;
	vector<SNode*>	m_DrawLIst;
	vector<SNode*>	m_LODDrawLIst;
	vector<vector<SNode*>>	m_LevelLIst;
	int m_iNumCell;
	int m_iPatchLodCount;
	int Face;
	DWORD*			m_IndexList;
public:
	bool	Build(SMap* m_Map);
	SNode*	BuildNode(SMap* m_Map);
	bool	Division(SNode* Node);
	SNode*	CreatorNode(SNode* Node, float tl, float tr, float bl, float br);
	void	CreatorIndexBuffer(SNode* Node, float row, float col);
	bool	Render(ID3D11DeviceContext*	pd3dContext);
	void	DrawCheck(SNode* Node);
	bool	Frame();
	void	BoungigBox(SNode* Node);
	Vector2 GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	void	GetDrowList();
	void	AddDrowList(SNode* Node);
	bool	Release() ;
public:
	//LOD ฐüทร
	void SetNeighvorNode(SNode* Node);
	void SetNeighvor(SNode* Node);
	DWORD GetLodSubIndex(SNode* Node);
	void  SetLOD(int iNumDepth);
	float GetExpansionRatio(Vector3 vCenter);
	DWORD GetLodType(SNode* pNode);
	int	  UpdateIndexList(SNode* pNode, DWORD dwCurentIndex, DWORD dwNumLevel);
	int   SetLodIndexBuffer(SNode* pNode, DWORD& dwCurentIndex, DWORD dwA, DWORD dwB, DWORD dwC, DWORD dwType);
public:
	SQuadTree();
	virtual ~SQuadTree();
};

