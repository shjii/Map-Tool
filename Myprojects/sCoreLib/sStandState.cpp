//#include "sStandState.h"
//
//void sStandState::Process(sObject* pPlayer, sAnimation& animat)
//{
//	sPoint ptDist = pPlayer->m_ptPos - m_pOwner->m_ptPos;
//	float fDistance = ptDist.Length();
//	if (fDistance < 1000 && m_pOwner->m_szName == L"Boos")
//	{
//		animat.SetAnimation(L"BOSSI");
//		m_pOwner->SetTransition(EVENT_POINTMOVE);
//	}
//	if (m_pOwner->HP <= 0)
//	{
//		if (m_pOwner->m_szName == L"NPC")
//		{
//			animat.SetAnimation(L"NPCA");
//			Delay += g_fSecondPerFrame;
//			if (Delay >= 1.0f)
//			{
//				m_pOwner->SetTransition(EVENT_POINTMOVE);
//			}
//		}
//		else
//		{
//			m_pOwner->SetTransition(EVENT_STANDDEAD);
//		}
//	}
//
//	if (m_pOwner->m_szName == L"human" )
//	{
//		animat.SetAnimation(L"humanWaiting");
//		sPoint ptDist = pPlayer->m_ptPos - m_pOwner->m_ptPos;
//		float fDistance = ptDist.Length();
//
//		if (fDistance < 650)
//		{
//			m_pOwner->SetTransition(EVENT_POINTMOVE);
//			//m_pOwner->SetTransition(EVENT_LOSTTARGET);
//		}
//
//	}
//	else if (m_pOwner->m_szName == L"NPC")
//	{
//		animat.SetAnimation(L"NPCW");
//	}
//	else if (m_pOwner->m_szName == L"Item")
//	{
//		animat.SetAnimation(L"Gun");
//	}
//	//if (fDistance < 50)
//	//{
//	// if (m_pOwner->m_szName == L"Item")
//	//{
//	//	animat.SetAnimation(L"Gun");
//	//}
//	else if (m_pOwner->m_szName == L"Tank")
//	{
//		animat.SetAnimation(L"TANKI");
//		sPoint ptDist = pPlayer->m_ptPos - m_pOwner->m_ptPos;
//		float fDistance = ptDist.Length();
//
//		if (fDistance < 650)
//		{
//			m_pOwner->SetTransition(EVENT_POINTMOVE);
//			//m_pOwner->SetTransition(EVENT_LOSTTARGET);
//		}
//	}
//
//	
//
//}
//sStandState::sStandState(sObject* owner) : sEnemyState(owner)
//{
//	m_dwState = STATE_STAND;
//}