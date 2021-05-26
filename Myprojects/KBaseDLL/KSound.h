#pragma once
#include "KStd.h"
#include "fomd/fmod.h"
#include "fomd/fmod.hpp"
#include "fomd/fmod_errors.h"
using namespace FMOD;
class KSound
{
public:
	FMOD::System*			m_pSystem;
	FMOD::Sound*			m_pSound;
	FMOD::Channel*			m_pChannel;
	TCHAR					m_szBuffer[256];
public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();
	bool  Load(System* pSystem,
		std::string szFileName,
		bool bPaused = true);
	void  Stop();
	FMOD_RESULT SetMode(FMOD_MODE mode = FMOD_LOOP_OFF);
	bool  Play(bool bPaused = false, bool bLoop = true);
	bool  PlayEffect(bool bLoop = false);

	void  Paused();
	void  Volume(float fVolume);
};

