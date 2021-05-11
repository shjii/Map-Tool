#pragma once
#include "SLod.h"

enum CornerPosition
{
	TL = 0,
	TR,
	BL,
	BR,
};
class SQuadTree : public SLod
{
public:
	vector<SNode*>	m_LeafNodeLIst;
	int		m_MaxDepth;
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
	bool	Release() ;
	void	GetUpdata(SCamera* C);
public:
	//LOD ฐüทร

public:
	SQuadTree();
	virtual ~SQuadTree();
};

