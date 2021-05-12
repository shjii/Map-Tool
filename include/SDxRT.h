#pragma once
#include "SDxObject.h"
class SDxRT
{
	ID3D11Device*	g_pd3dDevice;
public:
	int Width;
	int Height;
public:
	ID3D11RenderTargetView*	m_pRTV;
	ID3D11DepthStencilView*	m_pDSV;
	ID3D11ShaderResourceView* m_pSRV;
	D3D11_VIEWPORT			m_ViewPort;
	ID3D11RenderTargetView*	pSaveRTV;
	ID3D11DepthStencilView*	pSaveDSV;
	D3D11_VIEWPORT			vpSave;
	UINT					iNumView = 1;
public:
	void			Set(ID3D11Device*	pd3dDevice, int Width, int Height);
	virtual HRESULT	SetRenderTargetView();
	virtual HRESULT	SetDepthStencilView();
	virtual bool	SetViewport();
	virtual bool	Begin(ID3D11DeviceContext* pd3dContext);
	virtual bool	End(ID3D11DeviceContext* pd3dContext);
	virtual bool	Release();
	virtual void	ClearShaderResources(ID3D11DeviceContext* plmmediateContext);
};

