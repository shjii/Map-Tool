#include "SNode.h"

bool SNode::Release()
{
	return false;
}

SNode::SNode()
{
	m_Box.vAxis[0] = { 1.0f, 0.0f , 0.0f };
	m_Box.vAxis[1] = { 0.0f, 1.0f , 0.0f };
	m_Box.vAxis[2] = { 0.0f, 0.0f , 1.0f };
	m_ChildNode.clear();
	m_ChildNode.clear();
}


SNode::~SNode()
{
	if (!m_CornerList.empty()) m_CornerList.clear();

	for (int i = 0; i < m_ChildNode.size(); i++)
	{
		SAFE_DEL(m_ChildNode[i]);
	}
}