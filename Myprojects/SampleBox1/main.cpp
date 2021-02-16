#include "main.h"
//#include "SDxState.h"

bool main::Init()
{
	HRESULT hr;
	m_vCameraPos = { 10,10,-10 };
	m_vCameraTarget = { 0,0,0 };

	Vector3 p = m_vCameraPos;
	Vector3 t = m_vCameraTarget;
	Vector3 u = { 0,1,0 };
	m_matView.CreateViewLook(p, t, u);
	float fN = 1;
	float fF = 1000;
	float fFov = TBASIS_PI / 4.0f;
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_matProj.PerspectiveFovLH(fN, fF, fFov, fAspect);


	if (!m_Box.Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/kgca08.bmp"))
	{
		return false;
	}
	if (!m_Plane.Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/kgca08.bmp"))
	{
		return false;
	}
	if (!m_Line.Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/kgca08.bmp"))
	{
		return false;
	}
	return true;
}
bool main::Frame()
{
	xMatrix matScale;
	xMatrix matRotation;
	matScale.Scale(1, 1, 1);
	matRotation.YRotate(g_fGameTimer);
	m_matBoxWorld = matScale * matRotation;
	m_matPlaneWorld = matScale * matRotation;
	m_matPlaneWorld._41 = 3.0f;
	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_vCameraPos.z += 10.0f * g_fSecondPerFrame;
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_vCameraPos.z -= 10.0f * g_fSecondPerFrame;
	}
	if (g_Input.GetKey('F') == KEY_HOLD)
	{
		m_vCameraPos.y += 10.0f * g_fSecondPerFrame;
	}
	if (g_Input.GetKey('G') == KEY_HOLD)
	{
		m_vCameraPos.y -= 10.0f * g_fSecondPerFrame;
	}
	Vector3 u = { 0,1,0 };
	m_matView.CreateViewLook(m_vCameraPos, m_vCameraTarget, u);

	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		SDxState::m_FillMode = D3D11_FILL_WIREFRAME;
		SDxState::SetRasterizerState(m_pd3dDevice);
		//m_pd3dContext->RSSetState(m_pRSWireBack);
	}
	if (g_Input.GetKey('9') == KEY_PUSH)
	{
		SDxState::m_FillMode = D3D11_FILL_SOLID;
		SDxState::SetRasterizerState(m_pd3dDevice);
		//m_pd3dContext->RSSetState(m_pRSSolidBack);
	}
	if (g_Input.GetKey('8') == KEY_PUSH)
	{
		SDxState::m_CullMode = D3D11_CULL_BACK;
		SDxState::SetRasterizerState(m_pd3dDevice);
		//m_pd3dContext->RSSetState(m_pRSWireBack);
	}
	if (g_Input.GetKey('7') == KEY_PUSH)
	{
		SDxState::m_CullMode = D3D11_CULL_FRONT;
		SDxState::SetRasterizerState(m_pd3dDevice);
		//m_pd3dContext->RSSetState(m_pRSSolidBack);
	}
	return true;
}
bool main::Render()
{
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dContext->RSSetState(SDxState::m_pRS);
	m_pd3dContext->PSSetSamplers(0, 1, &SDxState::m_pWrapLinear);
	m_pd3dContext->OMSetDepthStencilState(SDxState::m_pDSS, 0);

	m_Box.SetMatrix(&m_matBoxWorld, &m_matView, &m_matProj);
	m_Box.Render(m_pd3dContext);

	m_Plane.SetMatrix(&m_matPlaneWorld, &m_matView, &m_matProj);
	m_Plane.Render(m_pd3dContext);

	m_Line.SetMatrix(NULL, &m_matView, &m_matProj);
	m_Line.Draw(m_pd3dContext,
		Vector3(0, 0, 0), Vector3(100, 0, 0), Vector4(1, 0, 0, 1));
	m_Line.Draw(m_pd3dContext,
		Vector3(0, 0, 0), Vector3(0, 100, 0), Vector4(0, 1, 0, 1));
	m_Line.Draw(m_pd3dContext,
		Vector3(0, 0, 0), Vector3(0, 0, 100), Vector4(0, 0, 1, 1));
	return true;
}
bool main::Release()
{
	m_Box.Release();
	m_Plane.Release();
	m_Line.Release();
	return true;
}