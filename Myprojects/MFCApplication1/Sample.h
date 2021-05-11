#pragma once
#include "TCore.h"
#include "SHeightMap.h"
#include "SQuadTree.h"
class Sample : public TCore
{
public:
	SMap*			m_Map;
	SQuadTree		m_QuadTree;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	bool Build(int tel, int cel, int ces, wstring tex);
};

