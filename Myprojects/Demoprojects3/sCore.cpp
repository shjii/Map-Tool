#include "sCore.h"

bool sCore::GameInit()
{
	m_bGameRun = true;
	m_sTime.Init();
	s_InPut.Init();
	Init();
	return true;
}
bool sCore::GameRelease()
{
	
	Release();
	m_sTime.Release();
	s_InPut.Release();
	return true;
}
bool sCore::GameRun()
{
	m_sTime.Frame();
	s_InPut.Frame();
	Frame();

	Render();
	m_sTime.Render();
	s_InPut.Render();

	Sleep(1);
	return true;
}
bool sCore::Run()
{
	GameInit();
	while (m_bGameRun)
	{
		if (MsgProcess() == false)
		{
			m_bGameRun = false;
		}
		else
		{
			GameRun();
		}
	}
	GameRelease();
	return true;
}