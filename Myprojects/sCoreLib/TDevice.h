#pragma once
#include "TWindow.h"
#include "SDxState.h"
class TDevice : public TWindow
{
public:
	ComPtr<IDXGIFactory>			m_pGIFactory;
	ComPtr<ID3D11Device>			m_pd3dDevice;
	ComPtr<ID3D11DeviceContext>		m_pImmediateContext;
	ComPtr<IDXGISwapChain>			m_pSwapChain;
	ComPtr<ID3D11RenderTargetView>	m_pRednerTargetView;
	ComPtr<ID3D11DepthStencilView>	m_pDSV;
	D3D11_VIEWPORT					m_ViewPort;
public:
	HRESULT		CreateGIFactory();
	HRESULT		CreateDevice();
	HRESULT		CreateSwapChain();
	HRESULT		SetRenderTargetView();
	HRESULT SetDepthStencilView();
	bool		SetViewport();
public:
	bool		Init();
	bool		Frame();
	bool		PreRender();
	bool		Render();
	bool		PostRender();
	bool		Release();
public:
	void		SetMode(bool bFullScreen);
	virtual void ResizeDevice(UINT w, UINT h) override;
	virtual HRESULT DeleteDXResource();
	virtual HRESULT CreateDXResource(UINT w, UINT h);
public:
	TDevice();
	virtual ~TDevice();
};

