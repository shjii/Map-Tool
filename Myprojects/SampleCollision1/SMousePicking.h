#pragma once
#include "SCollision.h"
class SMousePicking 
{
public:
	Vector3 Orig;
	Vector3 Dir;
public:
	bool	RayFrame(Matrix World,Matrix View,Matrix Proj);
};

