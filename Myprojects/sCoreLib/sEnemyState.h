#pragma once
#include "sObject.h"
#include "sFiniteState.h"
#include "sAnimation.h"
class sEnemyState
{
public:
	sObject* m_pOwner;
	DWORD m_dwState;
	float Delay;
public:
	/*virtual void Process(sObject* pPlayer, sAnimation& animat) = 0;*/
	sEnemyState(sObject* owner);
};

