#include "SMousePicking.h"

bool SMousePicking::RayFrame(Matrix World, Matrix View, Matrix Proj)
{
	POINT cursor;
	GetCursorPos(&cursor);
	ScreenToClient(g_hWnd, &cursor);

	Vector3 view;
	
	view.x = (((2.0f * cursor.x) / g_rtClient.right) - 1) / Proj._11;
	view.y = -(((2.0f * cursor.y) / g_rtClient.bottom) - 1) / Proj._22;
	view.z = 1.0f;
	
	Matrix ViewInvert = View.Invert();
	
	Orig = { 0.0f,0.0f,0.0f };
	Dir = view;

	Orig = Vector3::Transform(Orig,ViewInvert);
	Dir = Vector3::TransformNormal(Dir, ViewInvert);
	Dir.Normalize();

	return false;
}
bool SMousePicking::GetIntersection(Vector3 vStart, Vector3 vEnd, Vector3 vNormal, Vector3 v0, Vector3 v1, Vector3 v2)
{
	Vector3 vDirection = vEnd - vStart;
	float D = vNormal.Dot(vDirection); 
	float a0 = vNormal.Dot((v0 - vStart));
	float fT = a0 / D;
	if (fT < 0.0f || fT > 1.0f)
	{
		return false;
	}
	m_vIntersection = vStart + vDirection * fT;
	return true;
}

bool    SMousePicking::PointInPolygon(
	Vector3 vert,
	Vector3 faceNormal,
	Vector3 v0,
	Vector3 v1,
	Vector3 v2)
{
	Vector3 e0, e1, iInter, vNormal;
	// v0    v1
	//       v
	// v2    v3
	//
	//     v

	e0 = v2 - v1;
	e1 = v0 - v1;
	iInter = vert - v1;
	vNormal.Cross(e0, iInter);
	vNormal.Normalize();
	
	float fDot = faceNormal.Dot(vNormal);
	if (fDot < 0.0f) return false;
	vNormal.Cross(iInter, e1);
	vNormal.Normalize();
	fDot = faceNormal.Dot(vNormal);
	if (fDot < 0.0f) return false;

	//     v1
	//     v
	// v0      v2
	//
	//     v
	e0 = v0 - v2;
	e1 = v1 - v2;
	iInter = vert - v2;
	vNormal.Cross(e0, iInter);
	vNormal.Normalize();
	fDot = faceNormal.Dot(vNormal); 
	if (fDot < 0.0f) return false;
	vNormal.Cross(iInter, e1);
	vNormal.Normalize();
	fDot = faceNormal.Dot(vNormal);
	if (fDot < 0.0f) return false;
	return true;
};