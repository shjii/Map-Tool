#pragma once
#include "SMapLOD.h"
#include <queue>

class SQuadTree : public SMapLOD
{
public:
	enum QUADTREECORNER { tTL = 0, tTR = 1, tBL = 2, tBR = 3, };
	int				m_iRenderDepth;
	void			SetRenderDepth(DWORD dwDepth);
	SNode*					m_pRooSNode;
	queue<SNode*>			m_QuadTreeQueue;
	vector<TObject*>		m_DrawObjList;
	bool			m_bUsedIndexList;		// 정점 인덱스 사용하여 공간분할	
	int				m_iMaxDepthLimit;		// 노드 분할의 최대 한도
	void			SetMaxDepthLimit(int iMaxDepth) { m_iMaxDepthLimit = iMaxDepth; }
	int				m_iMaxDepth;			// 현 리프 노드의 레벨( 깊이 ) 개수
	float			m_fWidth;
	float			m_fHeight;
	// 최소	분할 크기
	float			m_fMinDivideSize;
	void			SetMinDivideSize(int iMinDivideSize) { m_fMinDivideSize = (float)iMinDivideSize; }
public:
	bool			Build(float fWidth, float fHeight);
	virtual bool	BuildTree(SNode* pNode);
	virtual bool	SubDivide(SNode* pNode);
	virtual bool	Release();
	virtual bool	PreFrame();
	virtual bool	Frame();
	SNode*			CreateNode(SNode* pParenSNode, float fTopLeft, float fTopRight, float fBottomLeft, float fBottomRight);

	int				AddObject(TObject* pObj);
	SNode*			FindNode(SNode* pNode, TObject* pObj);
	int				CheckRect(SNode* pNode, TObject* pObj);

	void			VisibleNode(SNode* pNode);
	void			VisibleObject(SNode* pNode);
	virtual void	DrawFindNode(SNode* pNode);

	DWORD			CheckSize(DWORD dwSize);
	void			SetNeighborNode(SNode* pNode);
	void			FindNeighborNode(SNode* pNode);
public:
	SQuadTree(void);
	virtual ~SQuadTree(void);
};
