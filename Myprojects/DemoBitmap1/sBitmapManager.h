#pragma once
#include "sBitmap.h"

class sBitmapManager : public GetSing<sBitmapManager>
{
private:
	map<wstring, sBitmap*> m_List;
	map<wstring, sBitmap*>::iterator m_iter;
public:
	sBitmap* Load(const TCHAR* filename);

	wstring szDefaultPath;

	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();

	sBitmap* GetPtr(wstring _name);

	sBitmapManager();
	~sBitmapManager();
};
#define g_BitMapMar sBitmapManager::GetInstance()

