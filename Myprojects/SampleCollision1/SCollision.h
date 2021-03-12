#pragma once
#include "SCamera.h"
class SCollision
{
public:
	Vector3 m_vIntersection;
public:
	bool	AABBToRay(S_BOX* Box, Vector3 Origin, Vector3 Dir = {0.0f,0.0f,0.0f});
	bool	SphereToRay(S_SPHERE* Spger, Vector3 Origin, Vector3 Dir = { 0.0f,0.0f,0.0f });
};

