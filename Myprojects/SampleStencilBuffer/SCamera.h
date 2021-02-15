#pragma once
#include "TStd.h"
#include "SShape.h"
class SCamera
{
public:
	float	m_pSpeed;
	Vector3 m_vCameraPos;
	Vector3 m_vCameraTarget;
	Matrix	m_matView;
	Matrix	m_matProj;
	Vector3	m_vLook;
	Vector3 m_vUp;
	Vector3 m_vRight;
	float	
};

