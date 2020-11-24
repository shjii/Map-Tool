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
	//
	m_pGamePlayer->surface = false;
	for (sObject* pObj : m_FloorList)
	{
		if (m_pGamePlayer->jump == false)
		{
				if (sCollision::Rect2Rect(pObj->m_rtCollide, m_pGamePlayer->m_rtCollide))
				{
					if (m_pGamePlayer->m_ptPos.y + (m_pGamePlayer->m_rtSrc.bottom / 2)
						<= pObj->m_ptPos.y + 5)
				{
					m_pGamePlayer->surface = true;
				}
			}
		}
	}
	//
	if (m_pGamePlayer != nullptr)
	{
		m_pGamePlayer->Frame();
		if (m_pGamePlayer->Middlecheck)
		{
			for (sObject* pfObj : m_FloorList)
			{
				pfObj->m_rtDesk.left -= m_pGamePlayer->m_fSpeed * g_fSecondPerFrame;
			}
			for (sObject* pObj : m_BackobjList)
			{
				if (pObj->m_rtDesk.left < -1100.0f)
				{
					m_pGamePlayer->m_EventScens = true;
				}
				pObj->m_rtDesk.left -= m_pGamePlayer->m_fSpeed * g_fSecondPerFrame;
			}
			for (sObject* pObj : m_objList)
			{
				pObj->m_ptPos.x -= m_pGamePlayer->m_fSpeed * g_fSecondPerFrame;
			}

		}
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
					if (pObj->m_bDead == false && pObj->HP > 0)
					{
						AddEffect(L"rtExplosion", project.p);
						project.m_bDead = true;
						pObj->Damage(1);
					}
				}
			}
	}
	return true;
}
sSceneInGame::sSceneInGame()
{
	m_bGameFinish = false;
	stage = 0;
}