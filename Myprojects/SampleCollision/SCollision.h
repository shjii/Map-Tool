#pragma once
#include "SCamera.h"
class SCollision
{
public:
	bool	AABBToRay(S_BOX* Box, Vector3 Origin, Vector3 Dir = {0.0f,0.0f,0.0f});
};

