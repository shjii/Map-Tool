#include "main.h"
#include <conio.h>
//void main()
//{
//	Netmain netMain;
//	string Ip;
//	string name;
//
//	printf("IP입력 :");
//	while (1)
//	{
//		int a = _getch();
//		system("cls");
//		if (a == '\r')
//		{
//			break;
//		}
//		else
//		{
//			Ip += a;
//		}
//		printf("IP입력 : %s", Ip.c_str());
//	}
//
//	printf("대화명입력 :");
//	while (1)
//	{
//		int a = _getch();
//		system("cls");
//		if (a == '\r')
//		{
//			break;
//		}
//		else
//		{
//			name += a;
//		}
//		printf("대화명입력 : %s", name.c_str());
//	}
//
//	if (netMain.NetInit(Ip, name) != true)
//	{
//		printf("x");
//	}
//
//	while (1)
//	{
//		netMain.NetFrame();
//
//		netMain.NetRender();
//
//	}
//
//	netMain.NetRelease();
//}


bool  main::Init()
{
	sMsg msg;
	msg.msg = L"[Information를 변경하세요]";
	int iTop = g_Write.m_TextList.size();
	msg.rt = { 0,iTop * 25,g_rtClient.right, g_rtClient.bottom };
	g_Write.push(msg);


	FILE* fp = nullptr;
	_wfopen_s(&fp, L"Information.txt", L"rt");
	TCHAR szBuffer[256] = { 0, };
	TCHAR szTemp[256] = { 0, };

	if (fp != nullptr)
	{
			_fgetts(szBuffer, _countof(szBuffer), fp);
			IP = szBuffer;
			_fgetts(szBuffer, _countof(szBuffer), fp);
			name = szBuffer;
	}

	fclose(fp);
	IP = IP.erase(IP.length() - 1, IP.length());
	name = name.erase(name.length() - 1, name.length());
	//g_Fsm.AddStateTrasition(STATE_STAND, EVENT_TIMEMOVE, STATE_MOVE);
	//g_Fsm.AddStateTrasition(STATE_STAND, EVENT_POINTMOVE, STATE_MOVE);
	//g_Fsm.AddStateTrasition(STATE_STAND, EVENT_FINDTARGET, STATE_ATTACK);
	//g_Fsm.AddStateTrasition(STATE_MOVE, EVENT_STOPMOVE, STATE_STAND);
	//g_Fsm.AddStateTrasition(STATE_MOVE, EVENT_FINDTARGET, STATE_ATTACK);
	//g_Fsm.AddStateTrasition(STATE_ATTACK, EVENT_LOSTTARGET, STATE_STAND);

	//m_iCurrentScene = 0;
	//m_SceneList.push_back(new TSceneIntro);
	//m_SceneList.push_back(new TSceneLobby);
	//m_SceneList.push_back(new TSceneInGame);
	//m_SceneList[0]->Load(L"../../data/script/Loading.txt");
	//m_SceneList[1]->Load(L"../../data/script/Lobby.txt");
	//m_SceneList[2]->Load(L"../../data/script/InGame.txt");
	//TScene::m_pCurrentScene = m_SceneList[2];
	//TScene::m_pCurrentScene->CreateScene();

	return NetInit();
}
bool  main::Frame()
{
	if (NetFrame() == false) return false;
	//TScene::m_pCurrentScene->Frame();
	//if (TScene::m_pCurrentScene->m_bGameFinish)
	//{
	//	m_bGameRun = false;
	//}
	return true;
}
bool  main::Release()
{
	if (NetRelease() == false) return false;
	//for (TScene* scene : m_SceneList)
	//{
	//	scene->Release();
	//}
	return true;
}
bool  main::Render()
{
	//TScene::m_pCurrentScene->Render();
	//if (TScene::m_pCurrentScene->m_bSceneChange == true)
	//{
	//	TScene::m_pCurrentScene->Reset();
	//	TScene::m_pCurrentScene = m_SceneList[TScene::m_pCurrentScene->m_iNextScene];
	//	TScene::m_pCurrentScene->CreateScene();
	//}
	return NetRender();
}

GAMERUN;