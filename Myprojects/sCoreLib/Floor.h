#pragma once
#include "sObject.h"
class sFloor :
	public sObject
{
public:
	//bool Frame()override;
	//bool Render()override;
	sFloor() 
	{
		m_iObjectType = 302;
	}

};

