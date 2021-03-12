#include "main.h"

bool main::Init()
{
	HRESULT hr;

	//m_Map.CreateHeightMap(g_pd3dDevice, g_pImmediateContext, L"../../data/map/heightMap513.bmp");

	SMapDesc desc;
	desc.iNumCols = 513;// m_Map.m_iNumCols;
	desc.iNumRows = 513;// m_Map.m_iNumRows;
	desc.fCellDistance = 1;
	desc.fScaleHeight = 10.0f;
	desc.szTexFile = L"../../data/map/grasshill.jpg";
	desc.szPS = L"ps.txt";
	desc.szVS = L"vs.txt";

	

	m_pMainCamera->Create(g_pd3dDevice);
	m_pMainCamera->CreateViewMatrix({ 0,100,-10 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_pMainCamera->CreateProjMatrix(1, 500, TBASIS_PI / 4.0f, fAspect);
	m_Map.CreateMap(g_pd3dDevice, g_pImmediateContext, desc);
	return true;
}
bool main::Frame()
{
	g_pImmediateContext->UpdateSubresource(
		m_Map.m_pVertexBuffer, 0, NULL, &m_Map.m_VertexList.at(0), 0, 0);

	m_Map.Frame();
	m_pMainCamera->Frame();
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
void main::Draw(SNode *Node, ID3D11DeviceContext * pd3dContext)
{
	m_Map.PreRender(pd3dContext);
	pd3dContext->IASetIndexBuffer(Node->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pd3dContext->DrawIndexed(Node->m_IndexList.size(), 0, 0);
}
bool main::Release()
{
	m_Map.Release();
	m_MinMap.Release();
	m_QuadTree.Release();
	return true;
}