#include "SItem.h"
#include "sScene.h"
#include "sCollision.h"

bool SItem::Init()
{
	sObject::Init();
	//m_sAnimation = new sAnimation;
	//m_sAnimation->SetOwner(this);
	//if (m_szName == L"GUN")
	//{
	//	m_sAnimation->SetAnimation(L"Gun");
	//}
	//else
	//{
	//	m_sAnimation->SetAnimation(L"Bomb");
	//}
	return true;
}

//중력 추가 , 자신의 위치와 플레이어의 위치 충돌 처리 

bool SItem::Frame()
{
	sObject::Frame();
	if (!surface)
	{
		GravitationalAction();
	}
	/*m_sAnimation->Frame();*/
	SetPos(m_ptPos);

	
	if (sCollision::Rect2Rect(m_rtCollide, sScene::m_pGamePlayer->m_rtCollide))
	{
		sScene::m_pGamePlayer->GetGun();
		m_bDead = true;
	}
	return true;
}

bool  SItem::Render()
{
	sObject::Render();
	/*m_sAnimation->Render();*/
	return true;
}