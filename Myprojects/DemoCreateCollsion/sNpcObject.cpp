#include "sNpcObject.h"

bool sNpcObject::Frame()
{
	if (m_bDead == true) return true;

	if (m_ptPos.x < g_rtClient.left)
	{
		m_ptDirection.x *= -1.0f;
		m_ptPos.x = g_rtClient.left;
	}
	if (m_ptPos.y < g_rtClient.top)
	{
		m_ptDirection.y *= -1.0f;
		m_ptPos.y = g_rtClient.top;
	}
	if (m_ptPos.x > g_rtClient.right)
	{
		m_ptDirection.x *= -1.0f;
		m_ptPos.x = g_rtClient.right;
	}
	if (m_ptPos.y > g_rtClient.bottom)
	{
		m_ptDirection.y *= -1.0f;
		m_ptPos.y = g_rtClient.bottom;
	}
	m_ptPos.x += m_ptDirection.x *
		m_fSpeed *
		g_fSecondPerFrame;
	m_ptPos.y += m_ptDirection.y *
		m_fSpeed *
		g_fSecondPerFrame;

	SetPosition(m_ptPos);
	return true;
}