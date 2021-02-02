//#include "sMoveState.h"
//
//void sMoveState::Process(sObject* pPlayer, sAnimation& animat)
//{
//	sPoint ptDist = pPlayer->m_ptPos - m_pOwner->m_ptPos;
//	float fDistance = ptDist.Length();
//	if (fDistance < 700 && m_pOwner->m_szName == L"Boos")
//	{
//		animat.SetAnimation(L"BOSSR");
//		if (fDistance < 600)
//		{
//			m_pOwner->SetTransition(EVENT_FINDTARGET);
//		}
//	}
//	if (m_pOwner->HP <= 0)
//	{
//		if (m_pOwner->m_szName == L"NPC")
//		{
//			animat.SetAnimation(L"NPCR");
//			if (sCollision::Rect2Rect(m_pOwner->m_rtCollide, pPlayer->m_rtCollide))
//			{
//				animat.SetAnimation(L"NPCC");
//				m_pOwner->SetTransition(EVENT_FINDTARGET);
//			}
//		}
//		else
//		{
//			m_pOwner->SetTransition(EVENT_MOVEDEAD);
//		}
//	}
//
//	if (m_pOwner->m_szName == L"human")
//	{
//		animat.SetAnimation(L"humanRun");
//		Delay += g_fSecondPerFrame;
//		if (Delay >= 1.0f)
//		{
//			Delay = 0.0f;
//			m_pOwner->SetTransition(EVENT_FINDTARGET);
//		}
//	}
//
//	if (m_pOwner->m_szName == L"Tank")
//	{
//		m_pOwner->SetTransition(EVENT_FINDTARGET);
//	}
//	float a = m_pOwner->m_ptPos.y;
//	m_pOwner->m_ptPos = m_pOwner->m_ptPos + ptDist.Normalize() * g_fSecondPerFrame* 100.0f;
//	m_pOwner->m_ptPos.y = a;
//	m_pOwner->SetPos(m_pOwner->m_ptPos);
//}
//sMoveState::sMoveState(sObject* owner) : sEnemyState(owner)
//{
//	m_dwState = STATE_MOVE;
//}
