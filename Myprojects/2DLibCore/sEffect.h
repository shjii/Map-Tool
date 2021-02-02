#pragma once
#include "sObject.h"

class sEffect : public sObject
{
public:
	vector<RECT> m_rtList;
	int m_iRectIndex;
	float m_fTimer;
	float m_fStep;
public:
	void SetSprite(vector<RECT>& list);
	void SetStep(float a);
	bool Frame();
	sEffect();
	virtual ~sEffect();
};

