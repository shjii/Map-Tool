#include "main.h"
//#include "SDxState.h"
Matrix* TD3DXMatrixShadow(Matrix *pout,
	Vector4 *plight,
	Vector4 *pplane)
{
	Vector4 Nplane;
	FLOAT dot;
	Nplane.Normalize();
	//D3DXPlaneNormalize(&Nplane, pplane);
	dot = Nplane.Dot(*plight);
	pout->m[0][0] = dot - Nplane.x * plight->x;
	pout->m[0][1] = -Nplane.x * plight->y;
	pout->m[0][2] = -Nplane.x * plight->z;
	pout->m[0][3] = -Nplane.x * plight->w;
	pout->m[1][0] = -Nplane.y * plight->x;
	pout->m[1][1] = dot - Nplane.y * plight->y;
	pout->m[1][2] = -Nplane.y * plight->z;
	pout->m[1][3] = -Nplane.y * plight->w;
	pout->m[2][0] = -Nplane.z * plight->x;
	pout->m[2][1] = -Nplane.z * plight->y;
	pout->m[2][2] = dot - Nplane.z * plight->z;
	pout->m[2][3] = -Nplane.z * plight->w;
	pout->m[3][0] = -Nplane.w * plight->x;
	pout->m[3][1] = -Nplane.w * plight->y;
	pout->m[3][2] = -Nplane.w * plight->z;
	pout->m[3][3] = dot - Nplane.w * plight->w;
	return pout;
}
Matrix main::CreateMatrixShadow(
	Vector4* pPlane,
	Vector4* pLight)
{
	Matrix mat;
	Vector4 plane, light;
	pPlane->Normalize();
	plane.x = pPlane->x * -1.0f;
	plane.y = pPlane->y * -1.0f;
	plane.z = pPlane->z * -1.0f;
	plane.w = pPlane->w * -1.0f;
	light = *pLight;// * -1.0f;
	float D = -(plane.Dot(light));
	mat._11 = plane.x * light.x + D;	mat._12 = plane.x * light.y;	mat._13 = plane.x * light.z;	mat._14 = plane.x * light.w;
	mat._21 = plane.y * light.x;	mat._22 = plane.y * light.y + D;	mat._23 = plane.y * light.z;	mat._24 = plane.y * light.w;
	mat._31 = plane.z * light.x;	mat._32 = plane.z * light.y;	mat._33 = plane.z * light.z + D;	mat._34 = plane.z * light.w;
	mat._41 = plane.w * light.x;	mat._42 = plane.w * light.y;	mat._43 = plane.w * light.z;	mat._44 = plane.w * light.w + D;
	return mat;
}
LRESULT	 main::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera == nullptr) return -1;
	m_pMainCamera->WndProc(hWnd, message, wParam, lParam);
	return -1;
}
bool main::Init()
{
	HRESULT hr;
	m_vDirValue = { 0,0,0,0 };

	Matrix matScale, matRotation;
	matScale = Matrix::CreateScale(1000, 1000, 1000);
	matRotation = Matrix::CreateRotationX(TBASIS_PI*0.5f);
	m_matPlaneWorld = matScale * matRotation;


	if (!m_BoxShape.Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/kgca08.bmp"))
	{
		return false;
	}
	if (!m_PlaneShape.Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/kgca08.bmp"))
	{
		return false;
	}
	if (!m_LineShape.Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/kgca08.bmp"))
	{
		return false;
	}
	m_ModelCamera.CreateViewMatrix({ 0,10,-10 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_ModelCamera.CreateProjMatrix(1, 1000, TBASIS_PI / 4.0f, fAspect);
	m_ModelCamera.Init();
	m_pMainCamera = &m_ModelCamera;
	return true;
}
bool main::Frame()
{
	Matrix matScale;
	Matrix matRotation;
	matScale = Matrix::CreateScale(1, 1, 1);
	matRotation = Matrix::CreateRotationY(g_fGameTimer);
	m_matBoxWorld = matScale * matRotation;
	m_matBoxWorld._42 = 3.0f;


	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		SDxState::m_FillMode = D3D11_FILL_WIREFRAME;
		SDxState::SetRasterizerState(m_pd3dDevice);
	}
	if (g_Input.GetKey('9') == KEY_PUSH)
	{
		SDxState::m_FillMode = D3D11_FILL_SOLID;
		SDxState::SetRasterizerState(m_pd3dDevice);
	}
	if (g_Input.GetKey('8') == KEY_PUSH)
	{
		SDxState::m_CullMode = D3D11_CULL_BACK;
		SDxState::SetRasterizerState(m_pd3dDevice);
	}
	if (g_Input.GetKey('7') == KEY_PUSH)
	{
		SDxState::m_CullMode = D3D11_CULL_FRONT;
		SDxState::SetRasterizerState(m_pd3dDevice);
	}

	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_pMainCamera->FrontMovement(1.0f);
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_pMainCamera->FrontMovement(-1.0f);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_pMainCamera->RightMovement(1.0f);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_pMainCamera->RightMovement(-1.0f);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_pMainCamera->UpMovement(1.0f);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_pMainCamera->UpMovement(-1.0f);
	}
	return true;
}
bool main::Render()
{
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dContext->RSSetState(SDxState::m_pRS);
	m_pd3dContext->PSSetSamplers(0, 1, &SDxState::m_pWrapLinear);
	m_pd3dContext->OMSetDepthStencilState(SDxState::m_pDSS, 0);
	m_BoxShape.SetMatrix(&m_matBoxWorld,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_BoxShape.Render(m_pd3dContext);

	Matrix matShadow;
	Vector4 PLANE = Vector4(0, 1, 0, -0.1f);
	//Vector4 vLightDir = Vector4(-10, 10, 0, 1);
	//matShadow = CreateMatrixShadow(&PLANE, &vLightDir);
	Vector3 vLightDir = Vector3(-10, 10, 0);
	vLightDir.Normalize();
	matShadow = Matrix::CreateShadow(vLightDir, PLANE);
	matShadow = m_matBoxWorld * matShadow;
	m_BoxShape.SetMatrix(&matShadow, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_BoxShape.Render(m_pd3dContext);

	m_PlaneShape.SetMatrix(&m_matPlaneWorld,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_PlaneShape.Render(m_pd3dContext);

	m_LineShape.SetMatrix(NULL, &m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_LineShape.Draw(m_pd3dContext,
		Vector3(0, 0, 0), Vector3(100, 0, 0), Vector4(1, 0, 0, 1));
	m_LineShape.Draw(m_pd3dContext,
		Vector3(0, 0, 0), Vector3(0, 100, 0), Vector4(0, 1, 0, 1));
	m_LineShape.Draw(m_pd3dContext,
		Vector3(0, 0, 0), Vector3(0, 0, 100), Vector4(0, 0, 1, 1));
	return true;
}
bool main::Release()
{
	m_BoxShape.Release();
	m_PlaneShape.Release();
	m_LineShape.Release();
	return true;
}