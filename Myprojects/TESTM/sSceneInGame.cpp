#include "sSceneInGame.h"

bool sSceneInGame::ReSet()
{
	m_pGamePlayer->m_ptPos = { 130, 400 };
	m_pGamePlayer->SetPos(m_pGamePlayer->m_ptPos);
	m_pGamePlayer->surface = false;
	m_pGamePlayer->m_EventScens = false;
	for (sObject* pObj : m_BackobjList)
	{
		pObj->m_rtDesk.left = 0;
	}
	for (sObject* pObj : m_objList)
	{
		pObj->m_bDead = true;
	}
	return true;
}

bool sSceneInGame::Render()
{
	sScene::Render();
	if (m_pGamePlayer != nullptr)
	{
		m_pGamePlayer->Render();
	}

	ZeroMemory(m_szBuffer, sizeof(WCHAR) * 256);
	_stprintf_s(m_szBuffer, L"\n ÃÑ¾Ë = %d, ÆøÅº  = %d",
		m_pGamePlayer->Gun,
		m_pGamePlayer->Bomb
		);
	OutputDebugString(m_szBuffer);

	return true;
}

bool sSceneInGame::Frame()
{
	if (g_KeyMap.bLeftClick > 2)
	{
		if (g_KeyMap.bSKey > 1)
		{
			sPoint a;
			a.x = g_InPut.GetPos().x;
			a.y = g_InPut.GetPos().y;
			Create(L"Tank", a);
		}
		else
		{
			sPoint a;
			a.x = g_InPut.GetPos().x;
			a.y = g_InPut.GetPos().y;
			Create(L"human", a);
		}
	}
	
	if (g_KeyMap.bRightClick > 2)
	{
		if (g_KeyMap.bSKey > 1)
		{
			sPoint a;
			a.x = g_InPut.GetPos().x;
			a.y = g_InPut.GetPos().y;
			Create(L"Boos", a);
		}
		else
		{
			sPoint a;
			a.x = g_InPut.GetPos().x;
			a.y = g_InPut.GetPos().y;
			Create(L"NPC", a);
		}

	}


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

		for (sbombInfo& project : m_pGamePlayer->m_sbombInfoList)
		{
			if (sCollision::Rect2Rect(project.m_rtCollide, pObj->m_rtCollide))
			{
				if (project.gravityCount == 3)
				{
					AddEffect(L"rtExplosion", project.p);
					project.m_bDead = true;
					//pObj->Damage(1);
				}
				else
				{
					project.p.y = pObj->m_ptPos.y - 10.0f;
					project.gravityCount += 1;
					project.gravity /= 2;
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
					//Create(L"Boos", { 1200 , 100});
				}
				pObj->m_rtDesk.left -= m_pGamePlayer->m_fSpeed * g_fSecondPerFrame;
			}
			for (sObject* pObj : m_objList)
			{
				pObj->m_ptPos.x -= m_pGamePlayer->m_fSpeed * g_fSecondPerFrame;
			}
			for (sObject* pObj : m_EventList)
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
			for (sbombInfo& project : m_pGamePlayer->m_sbombInfoList)
			{
				if (sCollision::Rect2Rect(project.m_rtCollide, pObj->m_rtCollide))
				{
					if (pObj->m_bDead == false && pObj->HP > 0)
					{
						AddEffect(L"rtExplosion", project.p);
						project.m_bDead = true;
						pObj->Damage(5);
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