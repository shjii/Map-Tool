#include "SNode.h"

SNode::SNode()
{
	m_Box.vCenter = Vector3(0.0f, 0.0f, 0.0f);
	m_Box.vMax = Vector3(1.0f, 1.0f, 1.0f);
	m_Box.vMin = Vector3(-1.0f, -1.0f, -1.0f);
	m_Box.vAxis[0] = Vector3(1.0f, 0.0f, 0.0f);
	m_Box.vAxis[1] = Vector3(0.0f, 1.0f, 0.0f);
	m_Box.vAxis[2] = Vector3(0.0f, 0.0f, 1.0f);
	m_Box.fExtent[0] = 1;
	m_Box.fExtent[1] = 1;
	m_Box.fExtent[2] = 1;
	m_ChildNode.clear();
}

SNode::~SNode()
{
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pConstantBuffer->Release();
}