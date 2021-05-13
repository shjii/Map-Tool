#include "SCollision.h"

bool SCollision::AABBToRay(S_BOX * Box, Vector3 Origin, Vector3 Dir)
{
	Vector3 Min;
	Min.x = (Box->vMin.x - Origin.x) / Dir.x;
	Min.y = (Box->vMin.y - Origin.y) / Dir.y;
	Min.z = (Box->vMin.z - Origin.z) / Dir.z;
	Vector3 Max;
	Max.x = (Box->vMax.x - Origin.x) / Dir.x;
	Max.y = (Box->vMax.y - Origin.y) / Dir.y;
	Max.z = (Box->vMax.z - Origin.z) / Dir.z;

	Vector3 reMin;
	reMin.x = min(Min.x, Max.x);
	reMin.y = min(Min.y, Max.y);
	reMin.z = min(Min.z, Max.z);
	Vector3 reMax;
	reMax.x = max(Min.x, Max.x);
	reMax.y = max(Min.y, Max.y);
	reMax.z = max(Min.z, Max.z);

	float minmax = min(min(reMax.x, reMax.y), reMax.z);
	float maxmin = max(max(reMin.x, reMin.y), reMin.z);

	if (minmax >= maxmin)
	{
		m_vIntersection = Origin + Dir * maxmin;
		return true;
	}
	return false;
}

bool SCollision::SphereToRay(S_SPHERE * Sphere, Vector3 Origin, Vector3 Dir)
{
	Vector3 vDir = Origin - Sphere->vCenter;
	float b = 2.0f * Dir.Dot(vDir);
	float c = vDir.Dot(vDir) - (Sphere->fRadius * Sphere->fRadius);
	float fDis = (b* b) - (4.0f*c);

	if (fDis < 0.0f)
	{
		return false;
	}
	fDis = sqrtf(fDis);
	float t0 = (-b + fDis) / 2.0f;
	float t1 = (-b - fDis) / 2.0f;

	if (t0 >= 0.0f)
	{
		m_vIntersection = Origin + Dir * t0;
		return true;
	}
	if (t1 >= 0.0f)
	{
		m_vIntersection = Origin + Dir * t1;
		return true;
	}
	return false;
}
bool SCollision::OBBtoRay(S_BOX* pBox, Vector3 org, Vector3 Dir)
{
	float t_min = -999999.0f;
	float t_max = 999999.0f;

	Vector3 diff = pBox->vCenter - org;

	for (int v = 0; v < 3; v++)
	{
		float f = pBox->vAxis[v].Dot(Dir);
		float s = pBox->vAxis[v].Dot(diff); 

		
		float t1 = (s - pBox->fExtent[v]) / f;
		float t2 = (s + pBox->fExtent[v]) / f;
		if (t1 > t2)
		{
			swap(t1, t2);
		}
		t_min = max(t_min, t1);
		t_max = min(t_max, t2);
		if (t_min > t_max)
			return false;

	}
	m_vIntersection = org + Dir * t_min;
	return true;
}