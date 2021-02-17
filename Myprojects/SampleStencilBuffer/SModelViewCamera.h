#pragma once
#include "SCamera.h"
#include "SFrustum.h"
class SModelViewCamera : public SCamera
{
public:
	SFrustum m_Frustum;
public:
	Vector3 m_vDownPt;
	Vector3 m_vCurrentPt;
	Quaternion m_qDown;
	Quaternion m_qNow;
	Matrix		m_mModelLastRot;
public:
	Matrix		GetRotationMatrix();
	Quaternion	QuatFromPoints(Vector3 v0, Vector3 v1);
	Vector3		ScreenToVector(float x, float y);
	void		OnBegin(int x, int y);
	void		OnMove(int x, int y);
	void		OnEnd();
	virtual int		WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual void	Update(Vector4 data) override;
	virtual bool	Frame() override;
public:
	SModelViewCamera();
	virtual ~SModelViewCamera();
};

