#pragma once
#include "SCollision.h"
class SMousePicking : public SCollision
{
public:
	Vector3 Orig;
	Vector3 Dir;
	//Vector3 m_vIntersection;
public:
	bool	RayFrame(Matrix World,Matrix View,Matrix Proj);
	bool	GetIntersection(
		Vector3 vStart,
		Vector3 vEnd,
		Vector3 vNormal,
		Vector3 v0,
		Vector3 v1,
		Vector3	v2);
	bool    PointInPolygon(
		Vector3 vert,
		Vector3 faceNormal,
		Vector3 v0,
		Vector3 v1,
		Vector3 v2);
};