#pragma once
#include "sCore.h"
#include "sEffect.h"

enum sObjectType {
	BACKGROUND = 0,
	UICONTROL ,
	USER  ,
	NPC 
};
typedef vector<RECT> RECT_ARRAY;

struct sEffectInfo
{
	POINT p;
	int id;
	int iMaxCount;
	bool bDead;
	float m_fLifeTime;
	float m_fTimer;
	float m_fStep;
	int m_iRectIndex;
	RECT m_rtSrc;
	RECT m_rtDesk;
	void Frame()
	{
		m_fLifeTime -= g_fSecondPerFrame;
		if (m_fLifeTime <= 0.0f)
		{
			bDead = true;
		}
		m_fTimer += g_fSecondPerFrame;
		if (m_fTimer >= m_fStep)
		{
			m_iRectIndex++;
			m_fTimer -= m_fStep;
		}
	}

	sEffectInfo()
	{
		bDead = false;
		m_fLifeTime = 1.0f;
		m_fTimer = 0.0f;
		m_fStep = 1.0f;
		m_iRectIndex = 0;
	}
};

class main : public sCore
{
public:
	vector<sEffect*> m_EffectList;
	vector<RECT_ARRAY> m_rtSpriteList;

	vector<sEffect> m_CurrentList;
	vector<sEffectInfo> m_List;
public:
	bool	GameDataLoad(const TCHAR* pszLoad);
	bool	Init()		override;
	bool	Frame()		override;
	bool	Render()	override;
	bool	Release()	override;
public:
	
};

