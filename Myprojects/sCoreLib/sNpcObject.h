#pragma once
#include "sStandState.h"
#include "sMoveState.h"
#include "sAttackState.h"
#include "sProjectile.h"
#include "sDeadState.h"
#include "sAnimation.h"
class sNpcObject :
	public sObject
{
public:
	bool HPC;
	vector<sbombInfo> m_sbombInfoList;
	/*sAnimation* m_NPCAnimation;*/
	sEffect*			m_pProjectile;
	vector<sProjectileInfo>	m_ProjectileList;
	//sEnemyState* m_pAction;
	//vector<sEnemyState*> m_pActionList;
	vector<sEffectInfo> m_EffectList;
public:
	bool Init();
	void FSM();
	void SetTransition(DWORD dwEevnt);
	void Attack(sObject* pPlayer);
	void BAttack(sObject* pPlayer);
	void Damage(int a);
	bool Render() ;
	void AddEffect(wstring name, sPoint pos);
public:
	bool Frame() override;
	sNpcObject()
	{
		m_iObjectType = 100;
	}
};

