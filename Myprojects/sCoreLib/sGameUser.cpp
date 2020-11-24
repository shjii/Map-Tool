#include "sGameUser.h"
#include "sInput.h"
#include "sObjectManager.h"
#include "sSoundManager.h"
bool sGameUser::Init()
{
	sObject::Init();
	m_pProjectile = (sEffect*)g_objectMgr.GetPtr(L"rtProjectile");
	m_psbomb = (sEffect*)g_objectMgr.GetPtr(L"rtBomb");
	m_Animation = new sAnimation;
	m_Animation->SetOwner(this);
	m_Animation->SetAnimationUP(L"UPWaiting");
	m_Animation->SetAnimationLO(L"LOWaiting");
	m_EventScens = false;
	Gun = 0;
	
	return true;
}

void sGameUser::GetGun()
{
	g_SoundMar.play("Ok.mp3");
	Gun += 50;
}

bool sGameUser::Frame()
{
	Middlecheck = false;
	m_fSpeed = 200.0f;
	m_Animation->SetAnimation(L"nullptr");
	if (AttackTime <= 0)
	{
		if (g_KeyMap.bAKey == 0 && g_KeyMap.bDKey == 0 && g_KeyMap.bWKey == 0 && g_KeyMap.bSKey == 0 && g_KeyMap.bSpace == 0
			&& g_KeyMap.bZKey == 0 && g_KeyMap.bXKey == 0)
		{
			if (Gun >= 1)
			{
				m_Animation->SetAnimationUP(L"UPHeavyMachineGun");

			}
			else
			{
				m_Animation->SetAnimationUP(L"UPWaiting");
			}
			
		}
	}
	else
	{
		AttackTime -= g_fSecondPerFrame;
	}

	if (g_KeyMap.bAKey == 0 && g_KeyMap.bDKey == 0 && g_KeyMap.bWKey == 0 && g_KeyMap.bSKey == 0 && g_KeyMap.bSpace == 0
		&& surface && g_KeyMap.bLeftClick == 0)
	{
		m_Animation->SetAnimationLO(L"LOWaiting");
	}

	m_direction.y = 0;
	if (jump)
	{
		m_fTimer += g_fSecondPerFrame;
		if (m_fTimer <= 0.3f)
		{
			m_ptPos.y -= m_fSpeed * g_fSecondPerFrame;
		}
		else
		{
			m_fTimer = 0;
			jump = false;
		}
	}
	else if (!jump && surface)
	{
		if (AttackTime <= 0)
		{
			if (g_KeyMap.bSKey > 0)
			{
				m_Animation->SetAnimation(L"DoDo");
				m_fSpeed = 100.0f;
			}
		} 

		if (g_KeyMap.bSpace > 0)
		{
			jump = true;
			surface = false;
			m_Animation->SetAnimationUP(L"UPJump");
			m_Animation->SetAnimationLO(L"LOJump");
		}
	}


	else if (!jump && !surface)
	{
		GravitationalAction();
	}
	SetPos(m_ptPos);

	if (g_KeyMap.bWKey > 0)
	{
		m_direction.y = -1;
		//if (AttackTime <= 0)
		{
			m_Animation->SetAnimationUP(L"UPUP");
		}
	}
	if (g_KeyMap.bDKey > 0)
	{
		m_direction.x = 1;
		if (m_ptPos.x >= g_rtClient.right / 2 && !m_EventScens)
		{
			Middlecheck = true;
		}
		else
		{
			m_ptPos.x += m_fSpeed * g_fSecondPerFrame;
			if (m_ptPos.x > g_rtClient.right - (m_rtSrc.right / 2))
			{
				m_ptPos.x = g_rtClient.right - (m_rtSrc.right / 2);
			}
		}
		SetPos(m_ptPos);
		if (AttackTime <= 0 && m_direction.y != -1)
		{
			m_Animation->SetAnimationUP(L"UPRightMove");
		}
		m_Animation->SetAnimationLO(L"RightMove");
	}
	if (g_KeyMap.bAKey > 0)
	{
		m_direction.x = -1;
		m_ptPos.x -= m_fSpeed * g_fSecondPerFrame;
		if (m_ptPos.x < g_rtClient.left + (m_rtSrc.right / 2))
		{
			m_ptPos.x = g_rtClient.left + (m_rtSrc.right / 2);
		}
		SetPos(m_ptPos);
	}

	if (Gun > 0)
	{
		m_pProjectile = (sEffect*)g_objectMgr.GetPtr(L"rtClash");
	}
	else
	{
		m_pProjectile = (sEffect*)g_objectMgr.GetPtr(L"rtProjectile");
	}

	if (m_pProjectile && g_KeyMap.bZKey == 3)
	{
		sProjectileInfo info;
		info.m_bDead = false;
		info.m_fLifeTime = 10.0f;
		info.id = 1;// rand() % 4;
		info.iMaxCount = m_pProjectile->m_rtList.size();
		info.m_fStep = 0.1f;
		info.p = { m_ptPos.x, m_ptPos.y };
		info.m_vDir = m_direction;
		if (m_direction.y != 0)
		{
			info.m_vDir.x = 0;
			info.m_vDir.y = m_direction.y;
		}
		m_ProjectileList.push_back(info);
		if (Gun > 0 && m_direction.y == 0)
		{
			m_Animation->SetAnimationUP(L"PistolAttack");
			Gun -= 1;
		}
		else if (Gun <= 0 && m_direction.y == 0)
		{
			m_Animation->SetAnimationUP(L"PistolAttack");
		}
		else if (Gun <= 0 && m_direction.y != 0) // À§·Î
		{
			m_Animation->SetAnimationUP(L"UpA");
		}
		
		if (m_fSpeed == 100.0f)
		{
			m_Animation->SetAnimation(L"DoA");
		}
		
		g_SoundMar.play("Gun.mp3");
		AttackTime = 0.5f;
	}
	else if (m_psbomb && g_KeyMap.bXKey == 3)
	{
		m_Animation->SetAnimationUP(L"UPBombAttack");
		if (Bomb > 0)
		{
			Bomb--;
			sbombInfo info;
			info.m_bDead = false;
			info.m_fLifeTime = 10.0f;
			info.id = 2;// rand() % 4;
			info.iMaxCount = m_psbomb->m_rtList.size();
			info.m_fStep = 0.1f;
			info.p = { m_ptPos.x, m_ptPos.y };
			info.m_vDir = m_direction;
			m_sbombInfoList.push_back(info);
			g_SoundMar.play("Gun.mp3");
		}
		else
		{

		}
		AttackTime = 0.5f;
	}

	
	for (std::vector<sProjectileInfo>::iterator iter = m_ProjectileList.begin();
		iter != m_ProjectileList.end();
		)
	{
		iter->Frame();
		if (iter->p.x < g_rtClient.left || iter->p.x > g_rtClient.right || iter->p.y > g_rtClient.bottom || iter->p.y < g_rtClient.top)
		{
			iter->m_bDead = true;
		}
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
		if (iter->p.x < g_rtClient.left || iter->p.x > g_rtClient.right || iter->p.y > g_rtClient.bottom || iter->p.y < g_rtClient.top)
		{
			iter->m_bDead = true;
		}
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
	m_Animation->Frame();
	
	m_Animation->SetP(m_direction.x);

	return false;
}

bool sGameUser::Render()
{
	sObject::Render();
	for (auto& pInfo : m_ProjectileList)
	{
		m_pProjectile->Set(pInfo.p, m_pProjectile->m_rtList[pInfo.m_iRectIndex]);
		pInfo.m_rtCollide = m_pProjectile->m_rtCollide;
 		m_pProjectile->Render();
	}
	for (auto& pInfo : m_sbombInfoList)
	{
		m_psbomb->Set(pInfo.p, m_psbomb->m_rtList[pInfo.m_iRectIndex]);
		pInfo.m_rtCollide = m_psbomb->m_rtCollide;
		m_psbomb->Render();
	}
	m_Animation->Render();
	return true;
}