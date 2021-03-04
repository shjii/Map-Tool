#pragma once
#include "SNode.h"
#include "SMap.h"
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
	SMap*	m_Map;
	SNode*	m_RootNode;
	int		m_MaxDepth;
public:
	bool	Build(SMap* m_Map);
	SNode*	BuildNode(SMap* m_Map);
	bool	Division(SNode* Node);
	SNode*	CreatorNode(SNode* Node, float tl, float tr, float bl, float br);
	void	CreatorIndexBuffer(SNode* Node, float row, float col);
	bool	Render(ID3D11DeviceContext*	pd3dContext);
	void	BoungigBox(SNode* Node);
public:

};

