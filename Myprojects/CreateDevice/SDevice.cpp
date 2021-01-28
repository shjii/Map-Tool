#include "SDevice.h"

bool SDevice::Init()
{ 
	D3D_FEATURE_LEVEL pFeatureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
	};
	IDXGIAdapter*	pAdapter = nullptr;
	D3D_DRIVER_TYPE	DriverType[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	HMODULE	Software = NULL;
	UINT	Flags = 0;
	UINT	FeatureLevels = sizeof(pFeatureLevels) / sizeof(pFeatureLevels[0]);
	UINT	SDKVersion = D3D11_SDK_VERSION;
	D3D_FEATURE_LEVEL OutputFeatureLevel;
	DXGI_SWAP_CHAIN_DESC pSwapChainDesc;
	ZeroMemory(&pSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	pSwapChainDesc.BufferDesc.Width = g_rtClient.right;
	pSwapChainDesc.BufferDesc.Height = g_rtClient.bottom;
	pSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	pSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	pSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;	
	pSwapChainDesc.SampleDesc.Count = 1;
	pSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	pSwapChainDesc.BufferCount = 1;
	pSwapChainDesc.OutputWindow = g_hWnd;
	pSwapChainDesc.Windowed = TRUE;

	HRESULT hr = S_OK;
	for (int iType = 0; iType < ARRAYSIZE(DriverType); iType++)
	{
		D3D11CreateDeviceAndSwapChain(
			pAdapter,
			DriverType[iType],
			Software,
			Flags,
			pFeatureLevels,
			FeatureLevels,
			SDKVersion,
			&pSwapChainDesc,
			&m_pSwapChain,
			&m_pd3dDevice,
			&OutputFeatureLevel,
			&m_pd3dContext);

		if (SUCCEEDED(hr))
		{
			break;
		}
	}
	if (FAILED(hr))
	{
		return false;
	}

	ID3D11Texture2D* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer);
	hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL,&m_pRedenrTargtView);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool SDevice::Frame()
{
	return true;
}

bool SDevice::PreRender()
{
	m_pd3dContext->OMSetRenderTargets(1, &m_pRedenrTargtView, NULL);
	float clearColor[] = { cosf(g_fGameTimer)*0.5f + 0.5f,
							-cosf(g_fGameTimer)*0.5f + 0.5f,
							sinf(g_fGameTimer)*0.5f + 0.5f,1 };
	m_pd3dContext->ClearRenderTargetView(m_pRedenrTargtView, clearColor);
	return true;
}

bool SDevice::Render()
{
	return true;
}

bool SDevice::PostRender()
{
	m_pSwapChain->Present(0,0);
	return true;
}

bool SDevice::Release()
{
	m_pd3dDevice->Release();
	m_pd3dContext->Release();
	m_pSwapChain->Release();
	m_pRedenrTargtView->Release();
	return true;
}
