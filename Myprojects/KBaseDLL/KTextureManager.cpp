#include "KTextureManager.h"

KTexture*    KTextureManager::Load(ID3D11Device* pd3dDevice, const TCHAR* filename)
{
	TCHAR drive[MAX_PATH] = { 0, };
	TCHAR dir[MAX_PATH] = { 0, };
	TCHAR name[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	_wsplitpath_s(filename, drive, dir, name, ext);

	std::wstring Dir = dir;
	std::wstring key;
	std::wstring loadfile;
	if (Dir.empty())
	{
		loadfile = m_szDefaultPath;
		loadfile += filename;
		key = filename;
	}
	else
	{
		loadfile = filename;
		key = name;
		key += ext;
	}
	KTexture* pData = GetPtr(key);
	if (pData != nullptr)
	{
		return pData;
	}


	pData = new KTexture;
	pData->Init();
	if (pData->Load(pd3dDevice, loadfile))
	{
		m_List.insert(make_pair(key, pData));
		return pData;
	}
	delete pData;
	return nullptr;
}
ID3D11ShaderResourceView*    KTextureManager::SRVLoad(ID3D11Device* pd3dDevice, const TCHAR* filename)
{
	TCHAR drive[MAX_PATH] = { 0, };
	TCHAR dir[MAX_PATH] = { 0, };
	TCHAR name[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	_wsplitpath_s(filename, drive, dir, name, ext);

	std::wstring Dir = dir;
	std::wstring key;
	std::wstring loadfile;
	if (Dir.empty())
	{
		loadfile = m_szDefaultPath;
		loadfile += filename;
		key = filename;
	}
	else
	{
		loadfile = filename;
		key = name;
		key += ext;
	}
	ID3D11ShaderResourceView* m_pTextureSRV;
	ID3D11Resource* texture;
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		pd3dDevice, loadfile.c_str(),
		NULL,
		&m_pTextureSRV);
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(
			pd3dDevice, loadfile.c_str(),
			NULL,
			&m_pTextureSRV);
		if (FAILED(hr))
		{
			return nullptr;
		}
	}
	return m_pTextureSRV;
}

KTexture*    KTextureManager::GetPtr(wstring filename)
{
	m_iter = m_List.find(filename);
	if (m_iter == m_List.end())
	{
		return nullptr;
	}
	return (*m_iter).second;
}

bool		KTextureManager::Init()
{
	m_pWhilteTexture = Load(g_pd3dDevice, L"../../data/bitmap/White.jpg");
	return true;
}
bool		KTextureManager::Frame()
{
	return true;
}
bool		KTextureManager::Render()
{
	return true;
}
bool		KTextureManager::Release()
{
	for (m_iter = m_List.begin();
		m_iter != m_List.end();
		m_iter++)
	{
		delete (*m_iter).second;
	}
	m_List.clear();
	return true;
}

KTextureManager::KTextureManager()
{
	m_szDefaultPath = L"../../data/bitmap/";
}

KTextureManager::~KTextureManager()
{
	Release();
}