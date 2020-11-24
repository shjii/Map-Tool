#include "sNpcObject.h"
#include "sScene.h"
#include "sFiniteStateMachine.h"
#include "sObjectManager.h"

void sNpcObject::AddEffect(wstring name, sPoint pos)
{
	sEffect* pEffect = (sEffect*)g_objectMgr.GetPtr(name);
	if (pEffect == nullptr) return;
	sEffectInfo info;
	info.m_csNmae = name;
	info.m_bDead = false;
	info.m_fLifeTime = 1.0f;
	info.iMaxCount = pEffect->m_rtList.size();
	info.m_fStep = 1.0f / pEffect->m_rtList.size();
	info.m_fStep = 0.1f;
	info.p = { pos.x , pos.y };
	m_EffectList.push_back(info);
}

bool sNpcObject::Init()
{
	sObject::Init();

	return true;
}

bool sNpcObject::Frame()
{
	if (!HPC)
	{
		HPC = true;
		if (m_szName == L"Boos")
		{
			HP = 20;
			m_pProjectile = (sEffect*)g_objectMgr.GetPtr(L"rtClash");

		}
		else if (m_szName == L"human")
		{
			m_pProjectile = (sEffect*)g_objectMgr.GetPtr(L"rtc");
			HP = 1;
		}
		else if (m_szName == L"Tank")
		{
			m_pProjectile = (sEffect*)g_objectMgr.GetPtr(L"TANKP");
			HP = 10;
		}
		else
		{
			HP = 1;
		}
	}

	m_NPCAnimation->SetOwner(this);
	if (m_bDead == true) return true;
	if (!jump && !surface)
	{
		
		GravitationalAction();
	}
	SetPos(m_ptPos);
	m_pAction->Process(sScene::m_pGamePlayer, *m_NPCAnimation);
	for (std::vector<sProjectileInfo>::iterator iter = m_ProjectileList.begin();
		iter != m_ProjectileList.end();
		)
	{
		iter->Frame();
		if (iter->m_bDead == true)
		{
			iter = m_ProjectileList.erase(iter);
			continue;
		}
		else
		{
			iter++;
		}
	}
	for (std::vector<sbombInfo>::iterator iter = m_sbombInfoList.begin();
		iter != m_sbombInfoList.end();
		)
	{
		iter->Frame();
		if (iter->m_bDead == true)
		{
			iter = m_sbombInfoList.erase(iter);
			continue;
		}
		else
		{
			iter++;
		}
	}
	for (std::vector<sEffectInfo>::iterator iter = m_EffectList.begin();
		iter != m_EffectList.end();
		)
	{
		iter->Frame();
		if (iter->m_bDead == true)
		{
			iter = m_EffectList.erase(iter);
			continue;
		}
		else
		{
			iter++;
		}
	}
	for (sProjectileInfo& project : m_ProjectileList)
	{
		if (sCollision::Rect2Rect(
			project.m_rtCollide,
			sScene::m_pGamePlayer->m_rtCollide))
		{
			sScene::m_pCurrentScene->AddEffect(L"rtExplosion", project.p);
			project.m_bDead = true;
		}
	}
	for (sbombInfo& project : m_sbombInfoList)
	{
		if (sCollision::Rect2Rect(
			project.m_rtCollide,
			sScene::m_pGamePlayer->m_rtCollide))
		{
			sScene::m_pCurrentScene->AddEffect(L"rtExplosion", project.p);
			project.m_bDead = true;
		}
	}
	m_NPCAnimation->Frame();
	return true;
}
void sNpcObject::FSM()
{
	m_pActionList.push_back(new sStandState(this));
	m_pActionList.push_back(new sMoveState(this));
	m_pActionList.push_back(new sAttackState(this));
	m_pActionList.push_back(new sDeadState(this));
	m_NPCAnimation = new sAnimation;
	m_pAction = m_pActionList[0];
	HP = 1;
	m_pProjectile = (sEffect*)g_objectMgr.GetPtr(L"rtProjectile");
}
void sNpcObject::SetTransition(DWORD dwEevnt)
{
	DWORD dwOutput = g_Fsm.GetTransition(m_pAction->m_dwState, dwEevnt);
	m_pAction = m_pActionList[dwOutput];
}

void sNpcObject::Attack(sObject* pPlayer)
{
	if (m_pProjectile)
	{
		sProjectileInfo info;
		info.m_bDead = false;
		info.m_fLifeTime = 10.0f;
		info.id = 3;// rand() % 4;
		info.iMaxCount = m_pProjectile->m_rtList.size();
		info.m_fStep = 0.1f;
		info.p = { m_ptPos.x, m_ptPos.y };
		sPoint vDir = pPlayer->m_ptPos - m_ptPos;
		if (vDir.x > 0)
		{
			vDir.x = 1;
		}
		else
		{
			vDir.x = -1;
		}
		vDir.y = 0;
		info.m_vDir = vDir;
		m_ProjectileList.push_back(info);
	}
}

void sNpcObject::BAttack(sObject* pPlayer)
{
	if (m_pProjectile)
	{
		
		sbombInfo info;
		info.m_bDead = false;
		info.m_fLifeTime = 10.0f;
		info.id = 3;// rand() % 4;
		info.iMaxCount = m_pProjectile->m_rtList.size();
		info.m_fStep = 0.1f;
		info.p = { m_ptPos.x, m_ptPos.y };
		sPoint vDir = pPlayer->m_ptPos - m_ptPos;
		if (vDir.x > 0)
		{
			vDir.x = 1;
		}
		else
		{
			vDir.x = -1;
		}
		vDir.y = 0;
		info.m_vDir = vDir;
		m_sbombInfoList.push_back(info);
	}
}

bool sNpcObject::Render()
{
	sObject::Render();
	for (auto& pInfo : m_ProjectileList)
	{
		m_pProjectile->Set(pInfo.p,
			m_pProjectile->m_rtList[pInfo.m_iRectIndex]);
		pInfo.m_rtCollide = m_pProjectile->m_rtCollide;
		m_pProjectile->Render();
	}
	for (auto& pInfo : m_sbombInfoList)
	{
		m_pProjectile->Set(pInfo.p,
			m_pProjectile->m_rtList[pInfo.m_iRectIndex]);
		pInfo.m_rtCollide = m_pProjectile->m_rtCollide;
		m_pProjectile->Render();
	}
	for (auto& pInfo : m_EffectList)
	{
		sEffect* pEffect = (sEffect*)g_objectMgr.GetPtr(pInfo.m_csNmae);
		pEffect->Set(pInfo.p, pEffect->m_rtList[pInfo.m_iRectIndex]);
		pEffect->Render();
	}
	m_NPCAnimation->Render();
	return true;
}

void sNpcObject::Damage(int a)
{
	HP -= a;
}