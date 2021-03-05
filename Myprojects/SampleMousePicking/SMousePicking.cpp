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
