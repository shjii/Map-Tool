#include "sObjectManager.h"

sObject* sObjectManager::Add(sObject* pAddObject)
{
	if (pAddObject == nullptr) return nullptr;
	sObject* pData = GetPtr(pAddObject->m_szName);
	if (pData != nullptr)
	{
		return pData;
	}
	m_List.insert(make_pair(pAddObject->m_szName, pAddObject));
	return nullptr;
}
sObject* sObjectManager::GetPtr(wstring filename)
{
	m_iter = m_List.find(filename);
	if (m_iter == m_List.end())
	{
		return nullptr;
	}
	return (*m_iter).second;
}
sObject* sObjectManager::Load(const TCHAR* filename, DWORD dwMaskColor)
{
	TCHAR drive[MAX_PATH] = { 0, };
	TCHAR dir[MAX_PATH] = { 0, };
	TCHAR name[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	_wsplitpath_s(filename, drive, dir, name, ext);

	wstring Dir = dir;
	wstring key;
	wstring loadfile;
	wstring maskfile;
	if (Dir.empty())
	{
		loadfile = m_szDfaultPath;
		loadfile += filename;

		maskfile = m_szDfaultPath;
		maskfile += name;
		maskfile += L"mask";
		maskfile += ext;
		key = filename;
	}
	else
	{
		loadfile = filename;
		maskfile = drive;
		maskfile += dir;
		maskfile += name;
		maskfile += L"mask";
		maskfile += ext;
		key = name;
		key += ext;
	}
	sObject* pData = GetPtr(key);

	if (pData != nullptr)
	{
		return pData;
	}

	pData = new sObject;
	pData->Init();
	pData->m_szName = key;
	if (pData->Load(loadfile.c_str(), maskfile.c_str(), dwMaskColor))
	{
		m_List.insert(make_pair(key, pData));
		return pData;
	}
	delete pData;
	return nullptr;
}
bool	sObjectManager::Init()
{
	return true;
}
bool	sObjectManager::Frame()
{
	return true;
}
bool	sObjectManager::Render()
{
	return true;
}
bool	sObjectManager::Release()
{
	for (m_iter = m_List.begin(); m_iter != m_List.end(); m_iter++)
	{
		delete (*m_iter).second;
	}
	m_List.clear();
	return true;
}

sObjectManager::sObjectManager()
{
	m_szDfaultPath = L"../../data/bitmap/";
}

sObjectManager::~sObjectManager()
{
	Release();
}