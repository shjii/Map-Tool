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
