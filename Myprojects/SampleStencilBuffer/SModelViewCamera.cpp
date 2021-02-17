#include "SModelViewCamera.h"

int SModelViewCamera::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int iMouseX = LOWORD(lParam);
	int iMouseY = HIWORD(lParam);

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		m_bDrag = true;
		OnBegin(iMouseX, iMouseY);
	}break;
	case WM_MOUSEMOVE:
	{
		if (m_bDrag)
		{
			OnMove(iMouseX, iMouseY);
		}
	}break;
	case WM_LBUTTONUP:
	{
		m_bDrag = false;
		OnEnd();
	}break;
	case WM_MOUSEWHEEL:
	{
		m_fWheelDelta += GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
	}break;
	}
	return -1;
}

Matrix SModelViewCamera::GetRotationMatrix()
{
	return Matrix::CreateFromQuaternion(m_qDown);
}

Quaternion SModelViewCamera::QuatFromPoints(Vector3 v0, Vector3 v1)
{
	Vector3 vCross;
	float fDot = v0.Dot(v1);
	vCross = v0.Cross(v1);
	return Quaternion(vCross.x, vCross.y, vCross.z, fDot);
}

Vector3 SModelViewCamera::ScreenToVector(float px, float py)
{
	float x = -((px / (float) g_rtClient.right) * 2.0f -1.0f);
	float y = ((py / (float) g_rtClient.bottom) * 2.0f - 1.0f);
	float z = 0.0f;
	float fDist = x * x + y * y;
	if (fDist > 1.0f)
	{
		float fScale = (1.0f / sqrtf(fDist));
		x *= fScale;
		y *= fScale;
	}
	else
	{
		z = sqrtf(1.0f - fDist);
	}
	return Vector3(x,y,z);
}

void SModelViewCamera::OnBegin(int x, int y)
{
	m_vDownPt = ScreenToVector(x,y);
	m_qDown = m_qNow;
}

void SModelViewCamera::OnMove(int x, int y)
{
	m_vCurrentPt = ScreenToVector(x,y);
	m_qDown = m_qNow * QuatFromPoints(m_vDownPt, m_vCurrentPt);
}

void SModelViewCamera::OnEnd()
{
}

void SModelViewCamera::Update(Vector4 data)
{
	Matrix mInvView = m_matView.Invert();
	mInvView._41 = mInvView._42 = mInvView._43 = 0.0f;

	Matrix mModelRotInv = m_mModelLastRot.Invert();

	Matrix mModlRot = GetRotationMatrix();
	m_matWorld = m_matWorld * m_matView * mModelRotInv * mModlRot *mInvView;
	m_mModelLastRot = mModlRot;

}

bool SModelViewCamera::Frame()
{
	Matrix mInvView = m_matView.Invert();
	mInvView._41 = mInvView._42 = mInvView._43 = 0.0f;

	Matrix mModelRotInv = m_mModelLastRot.Invert();

	Matrix mModlRot = GetRotationMatrix();
	m_matWorld = m_matWorld * m_matView * mModelRotInv * mModlRot *mInvView;
	m_matWorld._41 = m_matWorld._42 = m_matWorld._43 = 0.0f;
	m_mModelLastRot = mModlRot;

	UpdateVector();
	return true;
}

SModelViewCamera::SModelViewCamera()
{
	//m_qDown = Quaternion::Identity;
	//m_qNow = Quaternion::Identity;
}

SModelViewCamera::~SModelViewCamera()
{
}
