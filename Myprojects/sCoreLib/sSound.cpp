#include "sSound.h"
#include "sWrite.h"

bool sSound::Init()
{
	return true;
}
bool sSound::Frame()
{
	return true;
}
bool sSound::Render()
{
	ZeroMemory(m_szBuffer, sizeof(TCHAR) * 256);
	unsigned int ms = 0;
	unsigned int size = 0;
	m_pSound->getLength(&size, FMOD_TIMEUNIT_MS);
	m_pChannel->getPosition(&ms, FMOD_TIMEUNIT_MS);
	_stprintf_s(m_szBuffer, _T("전체 시간 [%02d:%02d:%02d]  :   경과시간 [%02d:%02d:%02d]"),
		size / 1000 / 60,
		size / 1000 % 60, 
		size / 10 % 60, 
		ms / 1000 / 60, 
		ms / 1000 % 60, 
		ms / 10 % 60);
	sMsg msg;
	msg.rt = g_rtClient;
	msg.rt.top = 45;
	msg.msg = m_szBuffer;
	g_Write.push(msg);
	return true;
}
bool sSound::Release() 
{
	m_pSound->release();
	return true;
}
bool sSound::Load(System* pSystem, string szFileName, bool bPused)
{
	m_pSystem = pSystem;
	FMOD_RESULT hr = pSystem->createSound(szFileName.c_str(), FMOD_DEFAULT, 0, &m_pSound);
	if (hr != FMOD_OK)
	{
		return false;
	}
	return true;
}
void sSound::Stop()
{
	if (m_pChannel != nullptr)
	{
		m_pChannel->stop();
	}
}
bool sSound::Play(bool bPaused, bool bLoop)
{
	bool play = false;
	if (m_pChannel != nullptr)
	{
		m_pChannel->isPlaying(&play);
	}
	if (play == false)
	{
		FMOD_RESULT hr = m_pSystem->playSound(m_pSound, NULL, bPaused, &m_pChannel);
		if (hr != FMOD_OK)
		{
			return false;
		}
		if (m_pChannel != nullptr)
		{
			m_pChannel->setVolume(0.5f);
		}
		if (bLoop)
		{
			m_pSound->setMode(FMOD_LOOP_NORMAL);
		}
		else
		{
			m_pSound->setMode(FMOD_LOOP_OFF);
		}
	}
	return true;
}
bool sSound::PlayEffect(bool bLoop)
{
	FMOD_RESULT hr = m_pSystem->playSound(m_pSound, NULL, false, &m_pChannel);
	if (hr != FMOD_OK)
	{
		return false;
	}
	if (m_pChannel != nullptr)
	{
		m_pChannel->setVolume(0.5f);
	}
	if (bLoop)
	{
		m_pSound->setMode(FMOD_LOOP_NORMAL);
	}
	else
	{
		m_pSound->setMode(FMOD_LOOP_OFF);
	}
	return true;
}

void sSound::Paused()
{
	bool paused = false;
	if (m_pChannel != nullptr)
	{
		m_pChannel->getPaused(&paused);
		m_pChannel->setPaused(!paused);
	}
}
void sSound::Volume(float fVolume)
{
	float fCurrentVolume = 0.0f;
	m_pChannel->getVolume(&fCurrentVolume);
	fCurrentVolume += fVolume * g_fSecondPerFrame; // 초당
	fCurrentVolume = min(1.0f, fCurrentVolume);
	fCurrentVolume = max(0.0f, fCurrentVolume);
	m_pChannel->setVolume(fCurrentVolume);
}