#pragma once
#include "TCore.h"
#include "SShape.h"

class main : public TCore
{
public:
	SShapeBox		m_Box;
	SShapePlane		m_Plane;
	SShapeLine		m_Line;
public:
	Matrix    m_matBoxWorld;
	Matrix    m_matPlaneWorld;
	Matrix    m_matView;
	Matrix    m_matProj;
public:

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};

GAME_RUN;

