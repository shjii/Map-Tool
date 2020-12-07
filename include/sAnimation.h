#pragma once
#include "sEffect.h"

class sAnimation : public sEffect
{
public:
	bool c;
	sObject* m_Owner;
	BLENDFUNCTION m_BlendFunction;
	sEffect* m_pUpperBody;
	sEffect* m_pLowerBody;
	sEffect* Body;
	int m_BodyIndex;
	HDC a;
	sPoint pos;
	int x;
	int Upx;
	int Lox;
	int y;
public:
	void SetOwner(sObject* _Owner);
	bool SetAnimationUP(wstring _name);
	bool SetAnimationLO(wstring _name);
	bool SetAnimation(wstring _name);
	bool SetP(sPoint a);
	
public:
	bool Frame();
	bool Render();
	sAnimation();
	~sAnimation();
};

