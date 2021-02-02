#pragma once
#include "sSound.h"

class sSoundManager : public GetSing<sSoundManager>
{
public:
	friend class GetSing<sSoundManager>;
private:
	System* m_pSystem;
	map<string, sSound*> m_List;
	map<string, sSound*>::iterator m_iter;
public:
	sSound* Load(const char* filename);
	sSound* GetPtr(string filename);
	string szDefaultPath;
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	bool		play(string name);
private:
	sSoundManager();
public:
	~sSoundManager();
};
#define g_SoundMar sSoundManager::GetInstance()

