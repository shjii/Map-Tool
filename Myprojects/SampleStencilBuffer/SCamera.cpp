#include "SCamera.h"

int SCamera::WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	int iRet = g_pWindow->MsgProc(hWnd, message, wParam, lParam);
	if (iRet > -1)
	{
		return iRet;
	}
	switch (message)
	{
	case WM_SIZE:
	{
		//if (SIZE_MAXHIDE)// 다른윈도우가 최대화 되어 가려지면
		//if (SIZE_MAXSHOW) // 최대화 되어 가려진 후 다시 복구될 때
		//if (SIZE_MAXIMIZED)// 최대화		
		//if (SIZE_RESTORED) // 크기가 변경되었을 때
		if (SIZE_MINIMIZED != wParam) // 최소화
		{
			UINT w = LOWORD(lParam);
			UINT h = HIWORD(lParam);
			g_pWindow->m_rtClient.right = w;
			g_pWindow->m_rtClient.bottom = h;
			g_rtClient = g_pWindow->m_rtClient;
			g_pWindow->ResizeDevice(w, h);
		}
	}break;
	case WM_MBUTTONDOWN:
	{
		m_bDrag = true;
		m_ptClick.x = LOWORD(lParam);
		m_ptClick.y = HIWORD(lParam);
	}return 0;
	case WM_MOUSEMOVE:
	{
		if (m_bDrag)
		{
			RECT rtWindow;
			GetWindowRect(hWnd, &rtWindow);
			rtWindow.left -= m_ptClick.x - LOWORD(lParam);
			rtWindow.top -= m_ptClick.y - HIWORD(lParam);
#if NDEBUG
			SetWindowPos(hWnd, HWND_TOPMOST, rtWindow.left, rtWindow.top, 0, 0, SWP_NOSIZE);//HWND_TOP
#else
			SetWindowPos(hWnd, HWND_NOTOPMOST, rtWindow.left, rtWindow.top, 0, 0, SWP_NOSIZE);
#endif
		}
	}return 0;
	case WM_MBUTTONUP:
	{
		m_bDrag = false;
	}return 0;
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			g_bActive = false;
			m_bDrag = false;
			SetWindowText(g_hWnd, L"INACTIVE");
		}
		else
		{
			::SetFocus(g_hWnd);
			g_bActive = true;
			SetWindowText(g_hWnd, L"ACTIVE");
		}

	}break;
	case WM_DESTROY:
		PostQuitMessage(0);// WM_QUIT
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
void SCamera::SetPos(Vector3 p)
{
	m_vCameraPos = p;
}

void SCamera::SetTarget(Vector3 p)
{
	m_vCameraTarget = p;
}

void SCamera::Update(Vector4 data)
{
	Matrix matRotation;
	matRotation = Matrix::CreateRotationY(data.y);
	Vector3 vLocalUp = { 0,1,0 };
	Vector3	vLocalLook = { 0,0,1 };
	vLocalUp = Vector3::Transform(vLocalUp, matRotation);
	vLocalLook = Vector3::Transform(vLocalLook, matRotation);
	vLocalLook.Normalize();
	vLocalUp.Normalize();
	float fHeight = m_vCameraPos.y;
	m_vCameraPos = m_vCameraTarget - vLocalLook * m_fDistance;
	m_vCameraPos.y = fHeight;
}

bool SCamera::CreateViewMatrix(Vector3 p, Vector3 t, Vector3 u)
{
	m_vCameraPos = p;
	m_vCameraTarget = t;
	m_fDistance = (m_vCameraPos - m_vCameraTarget).Length();
	m_matView = Matrix::CreateLookAt(p,t,u);
	UpdateVector();
	return true;
}

bool SCamera::CreateProjMatrix(float fN, float fF, float fFov, float fAspect)
{
	m_matProj = Matrix::CreatePerspectiveFieldOfView(fFov,fAspect,fN,fF);
	return true;
}

void SCamera::FrontMovement(float fDir)
{
	Vector3 vOffset = m_vLook * g_fSecondPerFrame * m_pSpeed * fDir;
	m_vCameraPos += vOffset;
}

void SCamera::RightMovement(float fDir)
{
	Vector3 vMove = m_vRight * g_fSecondPerFrame * m_pSpeed * fDir;
	m_vCameraPos += vMove;
}

void SCamera::UpMovement(float fDir)
{
	Vector3 vMove = m_vUp * g_fSecondPerFrame * m_pSpeed * fDir;
	m_vCameraPos += vMove;
}

void SCamera::FrontBase(float fDir)
{
	Vector3 vSide = { 0,0,1 };
	Vector3 vMove = vSide * g_fSecondPerFrame * m_pSpeed * fDir;
	m_vCameraPos += vMove;
	m_vCameraTarget += m_vLook * m_pSpeed;
}

void SCamera::RightBase(float fDir)
{
	Vector3 vSide = { 1,0,0 };
	Vector3 vMove = vSide * g_fSecondPerFrame * m_pSpeed * fDir;
	m_vCameraPos += vMove;
	m_vCameraTarget += m_vLook * m_pSpeed;
}

void SCamera::UpBase(float fDir)
{
	Vector3 vSide = { 0,1,0 };
	Vector3 vMove = vSide * g_fSecondPerFrame * m_pSpeed * fDir;
	m_vCameraPos += vMove;
	m_vCameraTarget += m_vLook * m_pSpeed;
}

void SCamera::UpdateVector()
{
	m_vLook.x = m_matView._13;
	m_vLook.y = m_matView._23;
	m_vLook.z = m_matView._33;
	m_vUp.x = m_matView._12;
	m_vUp.y = m_matView._22;
	m_vUp.z = m_matView._32;
	m_vRight.x = m_matView._11;
	m_vRight.y = m_matView._21;
	m_vRight.z = m_matView._31;

	m_vLook.Normalize();
	m_vUp.Normalize();
	m_vRight.Normalize();
}

bool SCamera::Init()
{
	return true;
}

bool SCamera::Frame()
{
	Vector3 vUp = { 0,1,0 };
	m_matView = Matrix::CreateLookAt(m_vCameraPos, m_vCameraTarget, vUp);
	UpdateVector();
	return true;
}

SCamera::SCamera()
{
	m_pSpeed = 30.0f;
	m_bDrag = false;
	m_fWheelDelta = 0;
}

SCamera::~SCamera()
{
}
