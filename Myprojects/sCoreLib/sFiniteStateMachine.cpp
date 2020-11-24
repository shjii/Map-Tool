#include "sFiniteStateMachine.h"

void sFiniteStateMachine::AddStateTrasition(DWORD dwState, DWORD dwEvent, DWORD dwOutputState)
{
	sFiniteState* pState = nullptr;
	auto ret = m_mapState.find(dwState);
	if (ret != m_mapState.end())
	{
		pState = ret->second;
	}
	else
	{
		pState = new sFiniteState(dwState);
		m_mapState[dwState] = pState;
	}
	pState->AddTransition(dwEvent, dwOutputState);
}
DWORD sFiniteStateMachine::GetTransition(DWORD dwState, DWORD dwEvent)
{
	return m_mapState[dwState]->m_mapList[dwEvent];
}
sFiniteStateMachine::~sFiniteStateMachine()
{
	for (auto list : m_mapState)
	{
		delete list.second;
	}
	m_mapState.clear();
}