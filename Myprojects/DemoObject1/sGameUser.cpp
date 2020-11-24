#include "sGameUser.h"

bool sGameUser::Frame()
{
	if (g_KeyMap.bDKey > 0)
	{
		m_ptPos.x += m_fSpeed * g_fSecondPerFrame;
	}
	if (g_KeyMap.bAKey > 0)
	{
		m_ptPos.x -= m_fSpeed * g_fSecondPerFrame;
	}
	if (g_KeyMap.bWKey > 0)
	{
		m_ptPos.y -= m_fSpeed * g_fSecondPerFrame;
	}
	if (g_KeyMap.bSKey > 0)
	{
		m_ptPos.y += m_fSpeed * g_fSecondPerFrame;
	}
	SetPosition(m_ptPos);
	return true;
}