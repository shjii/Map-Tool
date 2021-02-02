#pragma once
#include "sEnemyState.h"
class sStandState : public sEnemyState
{
public:
	void Process(sObject* pPlayer, sAnimation& animat);
	sStandState(sObject* owner);
};

