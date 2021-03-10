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
	float maxmax = max(max(reMin.x, reMin.y), reMin.z);

	if (minmax >= maxmax)
	{

		return true;
	}
	return false;
}
