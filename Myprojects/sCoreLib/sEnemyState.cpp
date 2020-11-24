#include "sEnemyState.h"


sEnemyState::sEnemyState(sObject* owner)
{
	Delay = 0.0f;
	m_pOwner = owner;
	m_dwState = 0;
}