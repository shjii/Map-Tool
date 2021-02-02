#include "sSoundManager.h"

bool	sSoundManager::play(string name)
{
	sSound* a = GetPtr(name);
	a->PlayEffect();
	return true;
}

sSound* sSoundManager::Load(const char* filename)
{
	CHAR drive[MAX_PATH] = { 0, };
	CHAR dir[MAX_PATH] = { 0, };
	CHAR name[MAX_PATH] = { 0, };
	CHAR ext[MAX_PATH] = { 0, };
	_splitpath_s(filename, drive, dir, name, ext);
	
	string Dir = dir;
	string key;
	string loadfile;
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
	sSound* pData = GetPtr(key);

	if (pData != nullptr)
	{
		return pData;
	}

	pData = new sSound;
	pData->Init();

	if (pData->Load(m_pSystem, loadfile))
	{
		m_List.insert(make_pair(key, pData));
			return pData;
	}
	return nullptr;
}
sSound* sSoundManager::GetPtr(string filename)
{
	m_iter = m_List.find(filename);
	if (m_iter == m_List.end())
	{
		return nullptr;
	}
	return (*m_iter).second;
}

bool sSoundManager::Init()
{
	FMOD_RESULT hr = System_Create(&m_pSystem);
	if (hr != FMOD_OK)
	{
		return false;
	}
	if (m_pSystem != nullptr)
	{
		hr = m_pSystem->init(32, FMOD_INIT_NORMAL, 0);
		if (hr != FMOD_OK)
		{
			return false;
		}
	}
	return true;
}
bool sSoundManager::Frame()
{
	m_pSystem->update();
	return true;
}
bool sSoundManager::Render()
{
	return true;
}
bool sSoundManager::Release()
{
	for (m_iter = m_List.begin(); m_iter != m_List.end(); m_iter++)
	{
		delete (*m_iter).second;
	}
	m_List.clear();

	m_pSystem->close();
	m_pSystem->release();
	return true;
}

sSoundManager::sSoundManager()
{
	szDefaultPath = "../../data/Sound/";
}

sSoundManager::~sSoundManager()
{

}