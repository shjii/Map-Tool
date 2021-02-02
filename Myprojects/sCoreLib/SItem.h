#pragma once
#include "sProjectile.h"
#include "sAnimation.h"
class SItem : public sObject
{
public:
	/*sAnimation* m_sAnimation;*/
public:
	bool Init();
	bool Frame();
	bool Render();
	SItem(){ m_iObjectType = 302; }
	
};

