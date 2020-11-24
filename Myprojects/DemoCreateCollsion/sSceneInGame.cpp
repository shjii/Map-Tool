#include "sSceneInGame.h"
bool sSceneInGame::Frame()
{
	sScene::Frame();
	for (sObject* pObj : m_objList)
	{
		if (sCollision::Rect2Rect(pObj->m_rtCollide , m_pGamePlayer->m_rtCollide))
		{
			m_CockData = true;
		}
	}
	return true;
}
sSceneInGame::sSceneInGame()
{
	m_CockData = false;
}