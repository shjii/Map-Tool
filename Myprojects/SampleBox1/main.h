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
	xMatrix    m_matBoxWorld;
	xMatrix    m_matPlaneWorld;
	xMatrix    m_matView;
	xMatrix    m_matProj;
public:
	Vector3	   m_vCameraPos;
	Vector3	   m_vCameraTarget;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};

GAME_RUN;

