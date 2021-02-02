#include "sEffect.h"

void sEffect::SetSprite(vector<RECT>& list)
{
	m_rtList = list;
}
bool sEffect::Frame()
{
	m_fTimer += g_fSecondPerFrame;
	if (m_fTimer >= m_fStep)
	{
		m_iRectIndex++;
		if (m_rtList.size() <= m_iRectIndex)
		{
			m_iRectIndex = 0;
		}
		m_fTimer -= m_fStep;
	}
	Set(m_rtList[m_iRectIndex], { (LONG)m_ptPos.x ,
		(LONG)m_ptPos.y, 
		m_rtList[m_iRectIndex].right,
		m_rtList[m_iRectIndex].bottom });
	return true;
}
void sEffect::SetStep(float a)
{
	m_fStep = a;
}
sEffect::sEffect()
{
	m_fTimer = 0.0f;
	m_fStep = 0.1f;
}
sEffect::~sEffect()
{

}