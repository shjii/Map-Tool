#include "SSkyBox.h"

bool	SSkyBox::Create(ID3D11Device* pDevice, T_STR szVS, T_STR szPS, T_STR	szTex)
{
	if (TObject::Create(pDevice,szVS,szPS,szTex) == false)
	{
		return 0;
	}
	m_pPSTextrueIndex.Attach(TBASIS_CORE_LIB::LoadPixelShaderFile(pDevice,szPS.c_str(), "PS_TextureIndex"));
	return true;
}
bool	SSkyBox::Render(ID3D11DeviceContext* pd3dContext)
{
	m_matWorld = Matrix::CreateScale(100.0f, 100.0f, 100.0f);
	Matrix matSkyBoxView = m_matView;
	matSkyBoxView._41 = 0;
	matSkyBoxView._42 = 0;
	matSkyBoxView._43 = 0;

	g_pImmediateContext->RSSetState(SDxState::m_pRSSolidNone);
	g_pImmediateContext->PSSetSamplers(0, 1, &SDxState::m_pWrapLinear);
	g_pImmediateContext->OMSetDepthStencilState(SDxState::m_pDSS, 0);
	/*TBASIS_CORE_LIB::ApplyDSS(pContext,
		TBASIS_CORE_LIB::TDxState::g_pDSSDepthDisable);

	ID3D11SamplerState* ppSamplerStates[2] = {
		TDxState::g_pSSClampPoint,
		TDxState::g_pSSWrapLinear };
	pContext->PSSetSamplers(0, 2, ppSamplerStates);*/

	SetMatrix(&m_matWorld, &matSkyBoxView, &m_matProj);
	SDxObject::Render(g_pImmediateContext);
	return true;
}
bool	SSkyBox::LoadTexture(T_STR szTex)
{
	HRESULT hr = S_OK;

	const TCHAR* g_szSkyTextures[] =
	{
		L"..\\..\\data\\sky\\st00_cm_front.bmp",
		L"..\\..\\data\\sky\\st00_cm_back.bmp",
		L"..\\..\\data\\sky\\st00_cm_right.bmp",
		L"..\\..\\data\\sky\\st00_cm_left.bmp",
		L"..\\..\\data\\sky\\st00_cm_up.bmp",
		L"..\\..\\data\\sky\\st00_cm_down.bmp"
	};
	int iNumTexture = sizeof(g_szSkyTextures) / sizeof(g_szSkyTextures[0]);

	for (int iTex = 0; iTex < iNumTexture; iTex++)
	{
		m_pTexSRV[iTex].Attach(TBASIS_CORE_LIB::CreateShaderResourceView(g_pd3dDevice, g_pImmediateContext, g_szSkyTextures[iTex]));
	}
	return true;
}
bool	SSkyBox::PostRender(ID3D11DeviceContext* pd3dContext)
{
	if (m_bRenderType)
	{
		for (int iTex = 0; iTex < MAX_SKYBOX_TEXTURE; iTex++)
		{
			if (m_pTexSRV[iTex] == nullptr) break;
			g_pImmediateContext->PSSetShaderResources(0,1,m_pTexSRV[iTex].GetAddressOf());
			g_pImmediateContext->PSSetShader(m_pPixelShader.Get(), NULL, 0);
			g_pImmediateContext->DrawIndexed(6, 6* iTex, 0);
		}
	}
	else
	{
		g_pImmediateContext->PSSetShaderResources(1, 6, m_pTexSRV[0].GetAddressOf());
		g_pImmediateContext->PSSetShader(m_pPSTextrueIndex.Get(), NULL, 0);
		g_pImmediateContext->DrawIndexed(36, 0, 0);
	}
	return true;
}

SSkyBox::SSkyBox()
{
	m_bRenderType = false;
}
SSkyBox::~SSkyBox()
{

}