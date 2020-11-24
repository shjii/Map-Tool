#include "main.h"

bool	main::Init()
{
	g_SoundMar.Init();
	m_pBG = g_SoundMar.Load("OnlyLove.mp3");
	m_pBG = g_SoundMar.Load("OnlyLove.mp3");
	m_pBG = g_SoundMar.Load("OnlyLove.mp3");
	m_pBG = g_SoundMar.Load("OnlyLove.mp3");
	m_pEffect = g_SoundMar.Load("Gun1.wav");
	m_pBG->Play();
	return true;
}
bool	main::Frame()
{	
	g_SoundMar.Frame();

	if (g_InPut.KeyCheck('1') == KEY_PUSH)
	{
		//m_pBG->Stop();
		m_pEffect->PlayEffect();
	}
	if (g_InPut.KeyCheck('2') == KEY_PUSH)
	{
		m_pBG->Play();
	}
	if (g_InPut.KeyCheck('3') == KEY_PUSH)
	{
		m_pBG->Paused();
	}
	if (g_InPut.KeyCheck('4') == KEY_HOLD)
	{
		// 시간의 동기화
		m_pBG->Volume(0.33f);
	}
	if (g_InPut.KeyCheck('5') == KEY_HOLD)
	{
		m_pBG->Volume(-0.33f);
	}
	return true;
}
bool	main::Render()
{
	g_SoundMar.Render();
	return true;
}
bool	main::Release()
{
	g_SoundMar.Release();
	return true;
}

GAMERUN;