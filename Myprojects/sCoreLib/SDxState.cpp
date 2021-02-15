#include "SDxState.h"

ID3D11RasterizerState*	SDxState::m_pRSSolidBack = nullptr;
ID3D11RasterizerState*	SDxState::m_pRSWireBack = nullptr;
ID3D11RasterizerState*	SDxState::m_pRS = nullptr;
ID3D11SamplerState*		SDxState::m_pWrapLinear = nullptr;
ID3D11DepthStencilState* SDxState::m_pDSS = nullptr;
D3D11_FILL_MODE			SDxState::m_FillMode = D3D11_FILL_SOLID;
D3D11_CULL_MODE			SDxState::m_CullMode = D3D11_CULL_NONE;

bool SDxState::Set(ID3D11Device * pd3dDevice)
{
	// DS STATE
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	ZeroMemory(&DepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	DepthStencilDesc.DepthEnable = TRUE;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	HRESULT hr = pd3dDevice->CreateDepthStencilState(
		&DepthStencilDesc, &m_pDSS);
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = 1;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 1;
	samplerDesc.MinLOD = FLT_MIN;
	samplerDesc.MaxLOD = FLT_MAX;
	hr = pd3dDevice->CreateSamplerState(&samplerDesc, &m_pWrapLinear);
	if (FAILED(hr))
	{
		return false;
	}
	m_FillMode = D3D11_FILL_SOLID;
	m_CullMode = D3D11_CULL_NONE;
	SetRasterizerState(pd3dDevice);

	D3D11_RASTERIZER_DESC rdesc;
	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = D3D11_FILL_SOLID;
	rdesc.CullMode = D3D11_CULL_BACK;
	hr = pd3dDevice->CreateRasterizerState(&rdesc, &m_pRSSolidBack);
	if (FAILED(hr))
	{
		return false;
	}

	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = D3D11_FILL_WIREFRAME;
	rdesc.CullMode = D3D11_CULL_BACK;
	hr = pd3dDevice->CreateRasterizerState(&rdesc, &m_pRSWireBack);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool SDxState::SetRasterizerState(ID3D11Device * pd3dDevice)
{
	HRESULT hr;
	// Rasterizer State
	if (m_pRS != nullptr) m_pRS->Release();
	D3D11_RASTERIZER_DESC rdesc;
	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = m_FillMode;
	rdesc.CullMode = m_CullMode;
	rdesc.DepthClipEnable = TRUE;
	hr = pd3dDevice->CreateRasterizerState(&rdesc, &m_pRS);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool SDxState::Release()
{
	m_pDSS->Release();
	m_pWrapLinear->Release();
	m_pRS->Release();
	m_pRSSolidBack->Release();
	m_pRSWireBack->Release();
	return false;
}
