#pragma once
#include "TStd.h"
#include "SShape.h"


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
	void	Normalize()
	{
		float fMag = sqrt(a*a + b * b + c * c);
		a = a / fMag;
		b = b / fMag;
		c = c / fMag;
		d = d / fMag;
	}
};
class SFrustum
{
public:
	SShapeBox m_FrustumObj;
	vector<PNCT_VERTEX>	m_VertexList;
	vector<SPlane>		m_Plane;
	Vector3 m_vFrustum[8];
	Matrix      m_mWorld;
	Matrix		m_mView;
	Matrix		m_mProj;
public:
	virtual bool Create(ID3D11Device*	pd3dDevice);
	virtual bool Frame();

	BOOL		ClassifyPoint(Vector3 v);
	BOOL		ClassifyBox(SShapeBox box);
	BOOL		CheckOBBInPlane(S_BOX* pBox);
	virtual	S_POSITION	CheckPoitionOBBInPlane(S_BOX* pBox);
	bool		CreateFrustum();
	void		SetMatrix(Matrix* pWorld, Matrix* pView, Matrix* pProj);
	void		ExtractPlanesD3D(vector<SPlane>& pPlanes, const Matrix& comboMatrix);
	SPlane		CreatePlane(Vector3 v0, Vector3 v1, Vector3 v2);
};

