#pragma once
#include "KStd.h"
#include "dxtk/WICTextureLoader.h"
#include "dxtk/DDSTextureLoader.h"
class KTexture
{
public:
	ID3D11ShaderResourceView* m_pTextureSRV;
public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();
	bool  Load(ID3D11Device* pd3dDevice, T_STR filename);
public:
	KTexture();
	virtual ~KTexture();
};

