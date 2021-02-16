#pragma once
#include "TStd.h"

class STexture
{
public:
	ID3D11ShaderResourceView* m_pTextureSRV;
public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();
	bool  Load(wstring filename);
public:
	STexture();
	virtual ~STexture();
};

