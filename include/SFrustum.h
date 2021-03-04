#pragma once
#include "TStd.h"
#include "SShape.h"

enum S_POSITION
{
	P_BACK = 0,    // Point is positioned behind plane
	P_FRONT,   // Point is positioned in front of plane	
	P_ONPLANE, // Point is positioned on plane
	P_SPANNING // Point is spanning plane
};
struct SPlane
{
	float a, b, c, d;
	void Create(Vector3 v0, Vector3 v1, Vector3 v2)
	{
		Vector3 n;
		Vector3 e0 = v1 - v0;
		Vector3 e1 = v2 - v0;
		n = e0.Cross(e1);
		n.Normalize();
		a = n.x;
		b = n.y;
		c = n.z;
		d = -n.Dot(v0);
	}
	void Create(Vector3 n, Vector3 v)
	{
		n.Normalize();
		a = n.x;
		b = n.y;
		c = n.z;
		d = -n.Dot(v);
	}
};
class SFrustum
{
public:
	SShapeBox m_FrustumObj;
	vector<PNCT_VERTEX>	m_VertexList;
	vector<SPlane>		m_Plane;
public:
	virtual bool Create(ID3D11Device*	pd3dDevice);
	virtual bool Frame();

	BOOL		ClassifyPoint(Vector3 v);
	BOOL		ClassifyBox(SShapeBox box);
	BOOL		CheckOBBInPlane(S_BOX* pBox);
	S_POSITION	CheckPoitionOBBInPlane(S_BOX* pBox);
};

