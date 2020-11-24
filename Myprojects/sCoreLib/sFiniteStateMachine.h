#pragma once
#include "sFiniteState.h"
class sFiniteStateMachine : public GetSing<sFiniteStateMachine>
{
	friend class GetSing<sFiniteStateMachine>;

	map<DWORD, sFiniteState*> m_mapState;
public:
	void AddStateTrasition(DWORD dwState, DWORD dwEvent, DWORD dwOutputState);
	DWORD GetTransition(DWORD dwState, DWORD dwEvent);

	sFiniteStateMachine() = default;
	~sFiniteStateMachine();
};
#define g_Fsm GetSing<sFiniteStateMachine>::GetInstance()

