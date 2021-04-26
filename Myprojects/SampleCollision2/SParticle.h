#pragma once
#include "TStd.h"
class SParticle
{
public:
	Vector3	m_vPos;
	Vector3 m_vAdd;
	Vector4	m_vColor;
	float	m_fTimer;
	float	m_fLifeTime;
public:
	SParticle();
	~SParticle();
};

