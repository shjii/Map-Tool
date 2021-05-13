#pragma once
#include "SCamera.h"
#include "TStd.h"
class SCollision
{
public:
	Vector3 m_vIntersection;
public:
	bool	AABBToRay(S_BOX* Box, Vector3 Origin, Vector3 Dir = {0.0f,0.0f,0.0f});
	bool	SphereToRay(S_SPHERE* Spger, Vector3 Origin, Vector3 Dir = { 0.0f,0.0f,0.0f });
	bool	OBBtoRay(S_BOX* pBox, Vector3 org, Vector3 Dir);
};

