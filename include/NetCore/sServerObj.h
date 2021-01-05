#pragma once
#include "sNetStd.h"
class sServerObj
{
public:
	CRITICAL_SECTION m_cs;
public:
	sServerObj();
	virtual~sServerObj();
};

