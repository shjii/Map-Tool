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
	STexture*		CreateLoad(LONG iWidth, LONG iHeight, const TCHAR* filename, int iBitCount = 24);
	STexture*		Load(const TCHAR* filename);
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
#define g_BitmapMgr STextureManager::GetInstance()
