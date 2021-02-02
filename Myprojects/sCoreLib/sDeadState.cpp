//#include "sDeadState.h"
//#include "sScene.h"
//void sDeadState::Process(sObject* pPlayer, sAnimation& animat)
//{
//		if (EVENTC != true)
//		{
//			if (m_pOwner->m_szName == L"NPC")
//			{
//				m_pOwner->CollisionCheck = true;
//				EVENTC = true;
//			}
//			if (m_pOwner->m_szName == L"human")
//			{
//				animat.SetAnimation(L"humanDeath");
//				EVENTC = true;
//			}
//			if (m_pOwner->m_szName == L"Tank")
//			{
//				animat.SetAnimation(L"TANKD");
//				m_pOwner->AddEffect(L"TANKDP", m_pOwner->m_ptPos);
//				EVENTC = true;
//			}
//			if (m_pOwner->m_szName == L"Boos")
//			{
//				animat.SetAnimation(L"BOSSD");
//				m_pOwner->CollisionCheck = true;
//				EVENTC = true;
//			}
//		}
//		else
//		{
//			m_pOwner->CollisionCheck = false;
//			Delay += g_fSecondPerFrame;
//			if (Delay >= 1.2f)
//			{
//				Delay = 0;
//				m_pOwner->m_bDead = true;
//			}
//		}
//	}
//
//sDeadState::sDeadState(sObject* owner) : sEnemyState(owner)
//{
//	m_dwState = STATE_DEAD;
//}