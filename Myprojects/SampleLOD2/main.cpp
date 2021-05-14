#include "main.h"

bool main::Init()
{
	HRESULT hr;
	SMapDesc desc;
	desc.iNumCols = 513;
	desc.iNumRows = 513;
	desc.fCellDistance = 1;
	desc.fScaleHeight = 100.0f;
	desc.szTexFile = L"../../data/map/grasshill.jpg";
	desc.szPS = L"ps.txt";
	desc.szVS = L"vs.txt";

	m_Map.CreateMap(g_pd3dDevice, g_pImmediateContext, desc);

	return true;
}
bool main::Frame()
{
	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		SDxState::m_FillMode = D3D11_FILL_WIREFRAME;
		SDxState::SetRasterizerState(g_pd3dDevice);
	}
	if (g_Input.GetKey('9') == KEY_PUSH)
	{
		SDxState::m_FillMode = D3D11_FILL_SOLID;
		SDxState::SetRasterizerState(g_pd3dDevice);
	}

	m_Map.Frame();
	return true;
}
bool main::Render()
{
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->RSSetState(SDxState::m_pRS);
	g_pImmediateContext->PSSetSamplers(0, 1, &SDxState::m_pWrapLinear);
	g_pImmediateContext->OMSetDepthStencilState(SDxState::m_pDSS, 0);

	m_Map.SetMatrix(NULL,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_Map.Render(g_pImmediateContext);

	
	return true;
}
bool main::PostRender()
{
	TCore::PostRender();
	return true;
}
bool main::Release()
{
	m_Map.Release();
	m_QuadTree.Release();
	return true;
}