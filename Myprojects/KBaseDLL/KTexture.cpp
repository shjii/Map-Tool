#include "KTexture.h"
bool  KTexture::Init()
{
	return true;
}
bool  KTexture::Load(ID3D11Device* pd3dDevice, T_STR filename)
{
	// load texture
	ID3D11Resource* texture;
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		pd3dDevice, filename.c_str(),
		NULL,
		&m_pTextureSRV);
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(
			pd3dDevice, filename.c_str(),
			NULL,
			&m_pTextureSRV);
		if (FAILED(hr))
		{
			return false;
		}
	}

	return true;
}
bool  KTexture::Frame()
{
	return true;
}
bool  KTexture::Release()
{
	if (m_pTextureSRV)
	{
		m_pTextureSRV->Release();
		m_pTextureSRV = nullptr;
	}
	return true;
}
bool  KTexture::Render()
{
	return true;
}

KTexture::KTexture()
{
	m_pTextureSRV = nullptr;
}

KTexture::~KTexture()
{
	Release();
}