#pragma once
#include "KSound.h"
class KSoundManager : public KSingleton<KSoundManager>
{
public:
	friend  class KSingleton<KSoundManager>;
private:
	FMOD::System*			m_pSystem;
	std::map<string, KSound*>			m_List;
	std::map<string, KSound*>::iterator	m_iter;
	std::string				m_szDefaultPath;
public:
	KSound*		Load(const char* filename);
	bool		PlayEffect(const char* filename,
		bool bLoop = false);
	KSound*		GetPtr(string filename);

	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
private:
	KSoundManager();
public:
	~KSoundManager();
};
#define g_SoundMgr KSoundManager::GetInstance()
