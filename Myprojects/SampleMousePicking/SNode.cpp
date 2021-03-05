#include "SNode.h"

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
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pConstantBuffer->Release();
}