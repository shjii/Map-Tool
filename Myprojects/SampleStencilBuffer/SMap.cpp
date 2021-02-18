#include "SMap.h"

bool SMap::CreateMap(ID3D11Device * pDevice, SMapDesc desc)
{
	m_MapDesc = desc;

	return true;
}

bool SMap::CreateVertexData()
{
	m_VertexList.resize();
	return true;
}

bool SMap::CreateIndexData()
{
	return true;
}

SMap::SMap()
{
}

SMap::~SMap()
{
}
