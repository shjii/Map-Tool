#pragma once
#include "sBitmap.h"

class sBitmapManager : public GetSing<sBitmapManager>
{
public:
	friend  class GetSing<sBitmapManager>;
private:
	map<wstring, sBitmap*> m_List;
	map<wstring, sBitmap*>::iterator m_iter;
	wstring szDefaultPath;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	sBitmap* Load(const TCHAR* filename);
	sBitmap* GetPtr(wstring _name);
	static HBITMAP CreateDIB(LONG iWidth, LONG iHeight, LPVOID* pPixeIData);
	static bool SaveFile(HBITMAP hBitmap, const TCHAR* strSaveFile);
	sBitmap* CreateLoad(LONG iWidth, LONG iHeight, const TCHAR* filename, int iBitCount);
private:
	sBitmapManager();
public:
	~sBitmapManager();
};
#define g_BitMapMar sBitmapManager::GetInstance()

