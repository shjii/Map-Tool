#pragma once
#include "sEffect.h"
typedef vector<RECT> RECT_APRAY;


struct sEffectInfo
{
	wstring m_csNmae;
	sPoint p;
	int id;
	int iMaxCount;
	bool m_bDead;
	float m_fLifeTime;
	float m_fTimer;
	float m_fStep;
	int m_iRectIndex;
	RECT m_rtSrc;
	RECT m_rtDesk;
	bool m_Completion;
	void Frame()
	{
		m_fLifeTime -= g_fSecondPerFrame;
		if (m_fLifeTime <= 0.0f)
		{
			m_bDead = true;
		}
		m_fTimer += g_fSecondPerFrame;
		
		if (iMaxCount - 1 <= m_iRectIndex)
		{
			m_Completion = true;
			m_iRectIndex = 0.0f;
			return;
		}
		else
		{
			m_Completion = false;
		}

		if (m_fTimer >= m_fStep)
		{
			m_iRectIndex++;
			m_fTimer -= m_fStep;
		}
	}
	sEffectInfo()
	{
		m_Completion = false;
		m_bDead = false;
		m_fLifeTime = 1.0f;
		m_fTimer = 0.0f;
		m_fStep = 1.0f;
		m_iRectIndex = 0;
	}
};

struct sProjectileInfo
{
	wstring m_csNmae;
	sPoint p;
	sPoint m_vDir;
	int id;
	int iMaxCount;
	bool m_bDead;
	float m_fLifeTime;
	float m_fTimer;
	float m_fStep;
	int m_iRectIndex;
	RECT m_rtSrc;
	RECT m_rtDesk;
	RECT m_rtCollide;
	void Frame()
	{
		m_fLifeTime -= g_fSecondPerFrame;
		if (m_fLifeTime <= 0.0f)
		{
			m_bDead = true;
			return;
		}
		m_fTimer += g_fSecondPerFrame;
		if (m_fTimer >= m_fStep)
		{
			m_iRectIndex++;
			if (iMaxCount - 1 <= m_iRectIndex)
			{
				m_iRectIndex = 0.0f;
			}
			m_fTimer -= m_fStep;
		}
		p = p + (m_vDir * g_fSecondPerFrame * 300.0f);
	}

	sProjectileInfo()
	{
		m_vDir = {1,0};
		m_bDead = false;
		m_fLifeTime = 1.0f;
		m_fTimer = 0.0f;
		m_fStep = 1.0f;
		m_iRectIndex = 0;
	}
};

struct sbombInfo
{
	wstring m_csNmae;
	sPoint p;
	sPoint m_vDir;
	float gravity;
	int id;
	int iMaxCount;
	int gravityCount;
	bool m_bDead;
	float m_fLifeTime;
	float m_fTimer;
	float m_fStep;
	int m_iRectIndex;
	RECT m_rtSrc;
	RECT m_rtDesk;
	RECT m_rtCollide;
	void Frame()
	{
		m_fLifeTime -= g_fSecondPerFrame;
		if (m_fLifeTime <= 0.0f)
		{
			m_bDead = true;
			return;
		}
		m_fTimer += g_fSecondPerFrame;
		if (m_fTimer >= m_fStep)
		{
			m_iRectIndex++;
			if (iMaxCount - 1 <= m_iRectIndex)
			{
				m_iRectIndex = 0.0f;
			}
			m_fTimer -= m_fStep;
		}
		gravity += 1.0f * g_fSecondPerFrame;
		p.x = p.x + (m_vDir.x * g_fSecondPerFrame * 200.0f);
		p.y = p.y - (1 * g_fSecondPerFrame * 300.0f - gravity);
	}

	sbombInfo()
	{
		gravityCount = 0;
		gravity = 0;
		m_vDir = { 1,1 };
		m_bDead = false;
		m_fLifeTime = 1.0f;
		m_fTimer = 0.0f;
		m_fStep = 1.0f;
		m_iRectIndex = 0;
	}
};

class sProjectile : public sEffect
{

};

