#include "sBitmapManager.h"

sBitmap* sBitmapManager::Load(const TCHAR* filename)
{
	TCHAR drive[MAX_PATH] = { 0, };
	TCHAR dir[MAX_PATH] = { 0, };
	TCHAR name[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	_wsplitpath_s(filename, drive, dir, name, ext);

	wstring Dir = dir;
	wstring key;
	wstring loadfile;
	if (Dir.empty())
	{
		loadfile = szDefaultPath;
		loadfile += filename;
		key = filename;
	}
	else
	{
		loadfile = filename;
		key = name;
		key += ext;
	}

	sBitmap* pData = GetPtr(key);
	if (pData != nullptr)
	{
		return pData;
	}

	pData = new sBitmap;
	pData->Init();

	if (pData->Load(loadfile))
	{
		m_List.insert(make_pair(key, pData));
		return pData;
	}
	delete pData;
	return nullptr;
}

sBitmap* sBitmapManager::GetPtr(wstring _name)
{
	m_iter = m_List.find(_name);
	if (m_iter == m_List.end())
	{
		return nullptr;
	}
		return (*m_iter).second;
}

bool sBitmapManager::Init()
{
	return true;
}
bool sBitmapManager::Frame()
{
	return true;
}
bool sBitmapManager::Render()
{
	return true;
}
bool sBitmapManager::Release()
{
	return true;
}

sBitmapManager::sBitmapManager()
{
	szDefaultPath = L"../../data/bitmap/";
}

sBitmapManager::~sBitmapManager()
{

}