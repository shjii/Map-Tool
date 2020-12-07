#include "main.h"


bool	main::Init()
{
	g_SoundMar.Init();
	m_pBG = g_SoundMar.Load("Back.mp3");
	m_pBG->Play();
	g_SoundMar.Load("Gun.mp3");
	g_SoundMar.Load("Ok.mp3");
	g_Fsm.AddStateTrasition(STATE_STAND, EVENT_POINTMOVE, STATE_MOVE);
	g_Fsm.AddStateTrasition(STATE_MOVE, EVENT_TIMEATTACK, STATE_STAND);
	g_Fsm.AddStateTrasition(STATE_MOVE, EVENT_FINDTARGET, STATE_ATTACK);
	g_Fsm.AddStateTrasition(STATE_ATTACK, EVENT_LOSTTARGET, STATE_MOVE);
	g_Fsm.AddStateTrasition(STATE_STAND, EVENT_STANDDEAD, STATE_DEAD);
	g_Fsm.AddStateTrasition(STATE_MOVE, EVENT_MOVEDEAD, STATE_DEAD);
	g_Fsm.AddStateTrasition(STATE_ATTACK, EVENT_ATTACKDEAD, STATE_DEAD);
	m_iCurrenScene = 0;
	m_SceneList.push_back(new sSceneIntro);
	m_SceneList.push_back(new sSceneLobby);
	m_SceneList.push_back(new sSceneInGame);
	m_SceneList.push_back(new sSceneEnd);
	m_SceneList[0]->Load(L"Loading.txt");
	m_SceneList[1]->Load(L"Lobby.txt");
	m_SceneList[2]->Load(L"InGame.txt");
	m_SceneList[3]->Load(L"END.txt");
	sScene::m_pCurrentScene = m_SceneList[0];

	return true;
}
bool	main::Frame()
{
	if (sScene::m_pCurrentScene->m_bSceneChange == true)
	{
		sScene::m_pCurrentScene->m_bSceneChange = false;
		sScene::m_pCurrentScene->ReSet();
		//if (m_iCurrenScene == 3)
		//{
		//	m_iCurrenScene = 0;
		//}
		sScene::m_pCurrentScene = m_SceneList[sScene::m_pCurrentScene->NextScene()];
	}
	sScene::m_pCurrentScene->Frame();
	return true;
}
bool	main::Render()
{
	sScene::m_pCurrentScene->Render();

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