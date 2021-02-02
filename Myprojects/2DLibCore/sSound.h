#pragma once
#include "sStd.h"
#include "fmod.h"
#include "fmod_errors.h"
#include "fmod.hpp"
using namespace FMOD;
class sSound
{
public:
	System* m_pSystem;
	Sound* m_pSound;
	Channel* m_pChannel;
	TCHAR m_szBuffer[256];
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	bool Load(System* pSystem, string szFileName, bool bPused = true);
	void Stop();
	bool Play(bool bPaused = false, bool bLoop = true);
	bool PlayEffect(bool bLoop = false);
	void Paused();
	void Volume(float fVolume);

};

