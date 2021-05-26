#pragma once
#include "KTexture.h"
class KTextureManager : public KSingleton<KTextureManager>
{
public:
	friend  class KSingleton<KTextureManager>;
private:
	std::map<wstring, KTexture*>				m_List;
	std::map<wstring, KTexture*>::iterator		m_iter;
	std::wstring								m_szDefaultPath;
public:
	KTexture*		m_pWhilteTexture;
public:
	KTexture*						Load(ID3D11Device* pd3dDevice, const TCHAR* filename);
	ID3D11ShaderResourceView*		SRVLoad(ID3D11Device* pd3dDevice, const TCHAR* filename);
	KTexture*						GetPtr(wstring filename);
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
private:
	KTextureManager();
public:
	~KTextureManager();
};
#define g_TextureMgr KTextureManager::GetInstance()
