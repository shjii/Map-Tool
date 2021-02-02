//#include "sAttackState.h"
//#include "sNpcObject.h"
//
//void sAttackState::Process(sObject* pPlayer, sAnimation& animat)
//{
//	if (m_pOwner->m_szName == L"Boos" && m_pOwner->HP > 0)
//	{
//		Delay -= g_fSecondPerFrame;
//		sPoint ptDist = pPlayer->m_ptPos - m_pOwner->m_ptPos;
//		float fDistance = ptDist.Length();
//		if (Delay <= 0)
//		{
//			if (fDistance < 100)
//			{
//				Delay = 1.0f;
//				animat.SetAnimation(L"BOSSAT");
//				pPlayer->death();
//			}
//			else
//			{
//				Delay = 0.5f;
//				animat.SetAnimation(L"BOSSA");
//				m_pOwner->Attack(pPlayer);
//			}
//		}
//	}
//	else if (m_pOwner->HP > 0 || m_pOwner->m_szName == L"NPC")
//	{
//		if (EVENTC != true)
//		{
//			if (m_pOwner->m_szName == L"NPC")
//			{
//				EVENT = EVENT_ATTACKDEAD;
//				Delay = 1.5f;
//				EVENTC = true;
//			}
//			if (m_pOwner->m_szName == L"human")
//			{
//				animat.SetAnimation(L"humanWaiting");
//					sPoint ptDist = pPlayer->m_ptPos - m_pOwner->m_ptPos;
//					float fDistance = ptDist.Length();
//					if (fDistance < 50)
//					{
//						Delay = 1.5f;
//						animat.SetAnimation(L"humanMeleeAttack");
//						if (sCollision::Rect2Rect(m_pOwner->m_rtCollide, pPlayer->m_rtCollide))
//						{
//							pPlayer->death();
//						}
//						EVENT = EVENT_LOSTTARGET;
//						EVENTC = true;
//					}
//					else
//					{
//						Delay = 1.5f;
//						animat.SetAnimation(L"humanDistanceAttack");
//						m_pOwner->BAttack(pPlayer);
//						//원거리 공격
//						EVENT = EVENT_LOSTTARGET;
//						EVENTC = true;
//					}
//			}
//			if (m_pOwner->HP > 0 && m_pOwner->m_szName == L"Tank")
//			{
//				Delay = 1.5f;
//				animat.SetAnimation(L"TANKA");
//				m_pOwner->Attack(pPlayer);
//				EVENT = EVENT_LOSTTARGET;
//				EVENTC = true;
//			}
//		}
//		else
//		{
//			Delay -= g_fSecondPerFrame;
//			if (Delay <= 0.0f)
//			{
//				EVENTC = false;
//				Delay = 0.0f;
//				m_pOwner->SetTransition(EVENT);
//			}
//		}
//	}
//	else
//	{
//		EVENTC = false;
//		m_pOwner->SetTransition(EVENT_ATTACKDEAD);
//	}
//
//}
//
//sAttackState::sAttackState(sObject* owner) : sEnemyState(owner)
//{
//	m_dwState = STATE_ATTACK;
//}