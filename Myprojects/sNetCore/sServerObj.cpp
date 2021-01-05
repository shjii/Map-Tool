#include "sServerObj.h"

sServerObj::sServerObj()
{
	InitializeCriticalSection(&m_cs);
}
sServerObj::~sServerObj()
{
	DeleteCriticalSection(&m_cs);
}