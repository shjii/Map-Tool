#include "sSceneInGame.h"
bool sSceneInGame::Render()
{
	sScene::Render();
	if (m_pGamePlayer != nullptr)
	{
		m_pGamePlayer->Render();
	}
	return true;
}

bool sSceneInGame::Frame()
{
	sScene::Frame();

	if (m_pGamePlayer != nullptr)
	{
		m_pGamePlayer->Frame();
	}
	for (sObject* pObj : m_objList)
	{
		if (sCollision::Rect2Rect(pObj->m_rtCollide , m_pGamePlayer->m_rtCollide))
		{
			m_bGameFinish = true;
		}
		for (sProjectileInfo& project : m_pGamePlayer->m_ProjectileList)
		{
			if (sCollision::Rect2Rect(project.m_rtCollide, pObj->m_rtCollide))
			{
				if (pObj->m_bDead == false)
				{
					AddEffect(L"rtExplosion", project.p);
				}
				pObj->m_bDead = true;
			}
		}
	}
	return true;
}
sSceneInGame::sSceneInGame()
{
	m_bGameFinish = false;
}