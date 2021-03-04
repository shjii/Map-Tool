#pragma once
//#include "SMapLOD.h"
#include <queue>
#include "SNode.h";
#include "SCamera.h"
class SQuadTree //: public SMapLOD
{
public:
	ID3D11Device*			m_pd3dDevice;
	enum QUADTREECORNER { tTL = 0, tTR = 1, tBL = 2, tBR = 3, };
	int				m_iRenderDepth;
	void			SetRenderDepth(DWORD dwDepth);
	SNode*					m_pRooSNode;
	queue<SNode*>			m_QuadTreeQueue;
	vector<SBaseObj*>		m_DrawObjList;
	vector< SNode* >		m_DrawNodeList;
	SCamera*				m_pCamera;
	bool			m_bUsedIndexList;		// ���� �ε��� ����Ͽ� ��������	
	int				m_iMaxDepthLimit;		// ��� ������ �ִ� �ѵ�
	void			SetMaxDepthLimit(int iMaxDepth) { m_iMaxDepthLimit = iMaxDepth; }
	int				m_iMaxDepth;			// �� ���� ����� ����( ���� ) ����
	float			m_fWidth;
	float			m_fHeight;
	// �ּ�	���� ũ��
	float			m_fMinDivideSize;
	void			SetMinDivideSize(int iMinDivideSize) { m_fMinDivideSize = (float)iMinDivideSize; }
public:
	bool			Build(float fWidth, float fHeight);
	virtual bool	BuildTree(SNode* pNode);
	virtual bool	SubDivide(SNode* pNode);
	virtual bool	Release();
	virtual bool	PreFrame();
	virtual bool	Frame();
	SNode*			CreateNode(SNode* pParentNode, float fTopLeft, float fTopRight, float fBottomLeft, float fBottomRight);

	int				AddObject(SBaseObj* pObj);
	SNode*			FindNode(SNode* pNode, SBaseObj* pObj);
	int				CheckRect(SNode* pNode, SBaseObj* pObj);

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

