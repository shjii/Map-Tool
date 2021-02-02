#include "TDevice.h"
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

	m_pd3dContext->OMSetRenderTargets(0, NULL, NULL);
	if (m_pRednerTargetView) m_pRednerTargetView->Release();
	DXGI_SWAP_CHAIN_DESC pSwapChainDesc;
	m_pSwapChain->GetDesc(&pSwapChainDesc);
	m_pSwapChain->ResizeBuffers(
		pSwapChainDesc.BufferCount,
		w,
		h,
		pSwapChainDesc.BufferDesc.Format,
		pSwapChainDesc.Flags);

	SetRenderTargetView();
	SetViewport();

	CreateDXResource(w,h);
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
	// 그래픽카드
	IDXGIAdapter*		 pAdapter = nullptr;
	D3D_DRIVER_TYPE		DriverType[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	HMODULE				Software = NULL; // 외부모듈 사용유무
	
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
			&m_pd3dContext);

		if (SUCCEEDED(hr))
		{
			break;
		}
	}
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
			m_pd3dDevice,
			&pSwapChainDesc,
			&m_pSwapChain);
	return hr;
}
HRESULT		TDevice::SetRenderTargetView()
{
	ID3D11Texture2D* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer);
		HRESULT hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL,
			&m_pRednerTargetView);
	if (pBackBuffer) pBackBuffer->Release();
	return hr;
}
bool TDevice::SetViewport()
{
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width  = g_rtClient.right;
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
	if (SetViewport()==false)
	{
		return false;
	}
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
	if (m_pd3dContext)
	{
		m_pd3dContext->OMSetRenderTargets(1, &m_pRednerTargetView, NULL);
		/*float clearColor[] = { cosf(g_fGameTimer)*0.5f + 0.5f,
								-cosf(g_fGameTimer)*0.5f + 0.5f,
								sinf(g_fGameTimer)*0.5f + 0.5f,1 };*/
		float clearColor[] = { 0,0,0,1 };
		m_pd3dContext->ClearRenderTargetView(m_pRednerTargetView, clearColor);
		m_pd3dContext->RSSetViewports(1, &m_ViewPort);
	}return true;
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
	m_pRednerTargetView->Release();
	m_pSwapChain->Release();
	m_pd3dContext->Release();	
	m_pd3dDevice->Release();
	m_pGIFactory->Release();
	return true;
}
TDevice::TDevice()
{
	m_pGIFactory	= nullptr;
	m_pd3dDevice = nullptr;
	m_pd3dContext = nullptr;
	m_pSwapChain = nullptr;
	m_pRednerTargetView = nullptr;
}
TDevice::~TDevice()
{

}