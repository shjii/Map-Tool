#include "TDevice.h"
namespace TBASIS_CORE_LIB
{
	ID3D11Device*  g_pd3dDevice = nullptr;
	ID3D11DeviceContext* g_pImmediateContext = nullptr;
}
void	TDevice::SetMode(bool bFullScreen)
{
	m_bFullScreen = bFullScreen;
	m_pSwapChain->SetFullscreenState(m_bFullScreen, NULL);
	if (m_bFullScreen == FALSE)
	{
		ShowWindow(m_hWnd, SW_SHOW);
	}
}
void TDevice::ResizeDevice(UINT w, UINT h)
{
	if (m_pd3dDevice == NULL)  return;

	DeleteDXResource();

	m_pImmediateContext->OMSetRenderTargets(0, NULL, NULL);
	if (m_pRednerTargetView) m_pRednerTargetView->Release();
	if (m_pDSV) m_pDSV->Release();

	DXGI_SWAP_CHAIN_DESC pSwapChainDesc;
	m_pSwapChain->GetDesc(&pSwapChainDesc);
	m_pSwapChain->ResizeBuffers(
		pSwapChainDesc.BufferCount,
		w,
		h,
		pSwapChainDesc.BufferDesc.Format,
		pSwapChainDesc.Flags);

	SetRenderTargetView();
	SetDepthStencilView();
	SetViewport();

	CreateDXResource(w, h);
}
HRESULT TDevice::DeleteDXResource()
{
	return S_OK;
}
HRESULT TDevice::CreateDXResource(UINT w, UINT h)
{
	return S_OK;
}
HRESULT TDevice::CreateGIFactory()
{
	if (m_pd3dDevice == NULL) return E_FAIL;
	HRESULT hr;
	IDXGIDevice * pDXGIDevice;
	hr = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);

	IDXGIAdapter * pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);

	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&m_pGIFactory);

	pDXGIDevice->Release();
	pDXGIAdapter->Release();
	return S_OK;
}
HRESULT		TDevice::CreateDevice()
{
	//D2D RT
	UINT	Flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL	pFeatureLevels[] =
	{
		//D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
	};
	// �׷���ī��
	IDXGIAdapter*		 pAdapter = nullptr;
	D3D_DRIVER_TYPE		DriverType[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	HMODULE				Software = NULL; // �ܺθ�� �������

	//UINT				FeatureLevels = ARRAYSIZE(pFeatureLevels);
	UINT				FeatureLevels = sizeof(pFeatureLevels) / sizeof(pFeatureLevels[0]);
	UINT				SDKVersion = D3D11_SDK_VERSION;
	D3D_FEATURE_LEVEL   OutputFeatureLevel;

	HRESULT hr = S_OK;
	for (int iType = 0; iType < ARRAYSIZE(DriverType); iType++)
	{
		hr = D3D11CreateDevice(
			pAdapter,
			DriverType[iType],
			Software,
			Flags,
			pFeatureLevels,
			FeatureLevels,
			SDKVersion,
			&m_pd3dDevice,
			&OutputFeatureLevel,
			&m_pImmediateContext);

		if (SUCCEEDED(hr))
		{
			break;
		}
	}
	g_pd3dDevice = m_pd3dDevice.Get();
	g_pImmediateContext = m_pImmediateContext.Get();
	return hr;
}
HRESULT		TDevice::CreateSwapChain()
{
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
	pSwapChainDesc.Windowed = true;
	//pSwapChainDesc.SwapEffect;
	pSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = m_pGIFactory->CreateSwapChain(
		m_pd3dDevice.Get(),
		&pSwapChainDesc,
		&m_pSwapChain);
	return hr;
}
HRESULT		TDevice::SetRenderTargetView()
{
	HRESULT hr = NULL;
	ID3D11Texture2D* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer);
		hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL,
			m_pRednerTargetView.GetAddressOf());
	if (pBackBuffer) pBackBuffer->Release();
	return hr;
}
HRESULT TDevice::SetDepthStencilView()
{
	// create depth texture
	ComPtr<ID3D11Texture2D> pTexture = nullptr;
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = g_rtClient.right;
	texDesc.Height = g_rtClient.bottom;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	HRESULT hr = m_pd3dDevice->CreateTexture2D(&texDesc, NULL, &pTexture);
	if (FAILED(hr))
	{
		return false;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = m_pd3dDevice->CreateDepthStencilView(
		pTexture.Get(),
		&dsvDesc,
		m_pDSV.GetAddressOf());

	//if (pTexture)pTexture->Release();
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool TDevice::SetViewport()
{
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = g_rtClient.right;
	m_ViewPort.Height = g_rtClient.bottom;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	return true;
}
bool TDevice::Init()
{
	if (FAILED(CreateDevice()))
	{
		return false;
	}
	if (FAILED(CreateGIFactory()))
	{
		return false;
	}
	if (FAILED(CreateSwapChain()))
	{
		return false;
	}
	if (FAILED(SetRenderTargetView()))
	{
		return false;
	}
	if (FAILED(SetDepthStencilView()))
	{
		return false;
	}
	if (SetViewport() == false)
	{
		return false;
	}

	SDxState::Set(m_pd3dDevice.Get());

	if (FAILED(m_pGIFactory->MakeWindowAssociation(m_hWnd,
		DXGI_MWA_NO_WINDOW_CHANGES |
		DXGI_MWA_NO_ALT_ENTER)))
	{
		return false;
	}
	return true;
}
bool TDevice::Frame()
{
	return true;
}
bool TDevice::PreRender()
{
	if (m_pImmediateContext.Get())
	{
		m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
		m_pImmediateContext->OMSetRenderTargets(1,
			m_pRednerTargetView.GetAddressOf(),
			m_pDSV.Get());
		float clearColor[] = { 0.3f,0.5f,0.23f,1 };
		m_pImmediateContext->ClearRenderTargetView(m_pRednerTargetView.Get(), clearColor);
		m_pImmediateContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
		m_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pImmediateContext->RSSetState(SDxState::m_pRS);
		m_pImmediateContext->PSSetSamplers(0, 1, &SDxState::m_pWrapLinear);
		m_pImmediateContext->OMSetDepthStencilState(SDxState::m_pDSS, 0);
	}
	return true;
}
bool TDevice::Render()
{
	return true;
}
bool TDevice::PostRender()
{
	if (m_pSwapChain)
	{
		m_pSwapChain->Present(0, 0);
	}
	return true;
}
bool TDevice::Release()
{
	SDxState::Release();
	return true;
}
TDevice::TDevice()
{
	m_pGIFactory = nullptr;
	m_pd3dDevice = nullptr;
	m_pImmediateContext = nullptr;
	m_pSwapChain = nullptr;
	m_pRednerTargetView = nullptr;
}
TDevice::~TDevice()
{

}