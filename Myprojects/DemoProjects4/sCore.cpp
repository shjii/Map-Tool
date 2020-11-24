#include "sCore.h"

bool sCore::GameInit()
{
	m_bGameRun = true;
	g_Timer.Init();
	g_InPut.Init();
	m_Graphic.Init();
	Init();
	return true;
}
bool sCore::GameRelease()
{
	Release();
	g_Timer.Release();
	g_InPut.Release();
	m_Graphic.Release();
	return true;
}
bool sCore::GameFrame()
{
	g_Timer.Frame();
	g_InPut.Frame();
	m_Graphic.Frame();
	Frame();
	return true;
}
bool sCore::PreRender()
{
	m_Graphic.PreRender();
	return true;
}
bool sCore::PostRender()
{
	m_Graphic.PostRender();
	return true;
}
bool sCore::GameRender()
{
	PreRender();
	Render();
	g_Timer.Render();
	g_InPut.Render();
	PostRender();
	return true;
}

bool sCore::GameRun()
{
	GameFrame();
	GameRender();
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