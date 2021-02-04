#pragma once
#include "sProjectile.h"
#include "sAnimation.h"

class sGameUser : public sObject
{
public:
	int Gun;
	int Bomb;
	int life;
	bool m_Dead;
	float InvincibleTime;
	bool m_EventScens;
	bool m_EventNPC;
	sPoint EventNPCP;
	float AttackTime;
	sAnimation* m_Animation;
	sEffect* m_pProjectile;
	sEffect* m_psbomb;
	vector<sProjectileInfo> m_ProjectileList;
	vector<sbombInfo> m_sbombInfoList;
	bool death();
public:
	void GetGun()override;
	bool Init()override;
	bool Frame()override;
	bool Render()override;
	sGameUser()
	{
		m_Dead = false;
		InvincibleTime = 0;
		life = 999999;
		Bomb = 10;
		 jump = false;
		 surface = false;
		 m_fTimer = 0.0f;
		m_iObjectType = 100;
		m_direction = {1, 0};
		Middlecheck = false;
	}
};
