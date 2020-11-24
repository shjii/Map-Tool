#include "sCore.h"

bool sCore::GameInit()
{
	m_bGameRun = true;
	Init();
	return true;
}
bool sCore::GameRelease()
{
	Release();
	return true;
}
bool sCore::GameRun()
{
	Frame();
	Render();
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