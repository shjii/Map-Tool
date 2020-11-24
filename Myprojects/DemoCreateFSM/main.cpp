#include "main.h"


bool	main::Init()
{
	g_Fsm.AddStateTrasition(STATE_STAND, EVENT_TIMEMOVE, STATE_MOVE);
	g_Fsm.AddStateTrasition(STATE_STAND, EVENT_POINTMOVE, STATE_MOVE);
	g_Fsm.AddStateTrasition(STATE_STAND, EVENT_FINDTARGET, STATE_ATTACK);
	g_Fsm.AddStateTrasition(STATE_MOVE, EVENT_STOPMOVE, STATE_STAND);
	g_Fsm.AddStateTrasition(STATE_MOVE, EVENT_FINDTARGET, STATE_ATTACK);
	g_Fsm.AddStateTrasition(STATE_ATTACK, EVENT_LOSTTARGET, STATE_STAND);

	m_iCurrenScene = 0;
	m_SceneList.push_back(new sSceneIntro);
	m_SceneList.push_back(new sSceneLobby);
	m_SceneList.push_back(new sSceneInGame);
	m_SceneList[0]->Load(L"Loading.txt");
	m_SceneList[1]->Load(L"Lobby.txt");
	m_SceneList[2]->Load(L"InGame.txt");
	m_pCurrentScene = m_SceneList[0];

	return true;
}
bool	main::Frame()
{
	if (m_pCurrentScene->m_bSceneChange == true)
	{
		m_pCurrentScene = m_SceneList[m_iCurrenScene++];
	}
	m_pCurrentScene->Frame();
	return true;
}
bool	main::Render()
{
	m_pCurrentScene->Render();
	return true;
}
bool	main::Release()
{
	for (sScene* scene : m_SceneList)
	{
		scene->Release();
	}
	return true;
}

GAMERUN;