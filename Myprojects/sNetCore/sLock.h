#pragma once
#include "sServerObj.h"
class sLock
{
	sServerObj* m_pServerObj;
public:
	sLock(sServerObj* pObj);
	virtual~sLock();
};

