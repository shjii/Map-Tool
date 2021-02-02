#include "sFiniteState.h"

void sFiniteState::AddTransition(DWORD dwEcent, DWORD dwOutputStatee)
{
	m_mapList[dwEcent] = dwOutputStatee;
}