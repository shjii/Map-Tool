#pragma once
#include "sStd.h"

enum sFSMState
{
	STATE_STAND = 0,
	STATE_MOVE,
	STATE_ATTACK,
	STATE_DEAD,
	STATE_COUNT,
};

enum sFSMEvent
{
	EVENT_STOPMOVE = 0,
	EVENT_POINTMOVE,
	EVENT_FINDTARGET,
	EVENT_LOSTTARGET,
	EVENT_TIMEATTACK,
	EVENT_STANDDEAD,
	EVENT_MOVEDEAD,
	EVENT_ATTACKDEAD,
	EVENT_COUNT,
};

// 현재 상태 -> 이벤트 -> 변경 상태
class sFiniteState
{
public:
	map<DWORD, DWORD> m_mapList;
	DWORD m_dwState;
public:
	void AddTransition(DWORD dwEcent, DWORD dwOutputStatee);
public:
	sFiniteState(DWORD dwState)
	{
		m_dwState = dwState;
	}
};

