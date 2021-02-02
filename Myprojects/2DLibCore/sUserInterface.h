#pragma once
#include "sObject.h"
#include "sInPut.h"

class sUserInterface : public sObject
{
public:
	int m_iState;
	sUserInterface() : m_iState(0)
	{
		m_iObjectType = 300;
	}
};


class sButton : public sUserInterface
{
public:
	bool Frame();

	bool Render();
	
	sButton()
	{
		m_iObjectType = 301;
	}
};
