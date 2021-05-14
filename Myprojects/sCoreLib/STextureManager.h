#pragma once
#include "STexture.h"
class STextureManager : public TSingleton<STextureManager>
{
public:
	friend  class TSingleton<STextureManager>;
private:
	std::map<wstring, STexture*>				m_List;
	std::map<wstring, STexture*>::iterator	m_iter;
	std::wstring				m_szDefaultPath;
public:
	STexture*		m_pWhilteTexture;
public:
	STexture*		Load(ID3D11Device* pd3dDevice, const TCHAR* filename);
	ID3D11ShaderResourceView*    SRVLoad(ID3D11Device* pd3dDevice, const TCHAR* filename);
	STexture*		GetPtr(wstring filename);
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
private:
	STextureManager();
public:
	~STextureManager();
};
#define g_TextMgr STextureManager::GetInstance()
