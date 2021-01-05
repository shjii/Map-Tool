#include "sLock.h"

sLock::sLock(sServerObj* pObj) : m_pServerObj(pObj)
{
	if (m_pServerObj == nullptr) return;
	EnterCriticalSection(&m_pServerObj->m_cs);
}
sLock::~sLock()
{
	if (m_pServerObj == nullptr) return;
	LeaveCriticalSection(&m_pServerObj->m_cs);
}