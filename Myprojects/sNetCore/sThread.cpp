#include "sThread.h"

void sThread::CreateThread()
{
	if (m_bStarted == true) return;
	m_hThread = _beginthreadex(NULL, 0, HandleRunner, (LPVOID)this, 0, &m_iThreadID);
	m_bStarted = true;
}

unsigned int __stdcall sThread::HandleRunner(LPVOID arg)
{
	sThread* pThread = (sThread*)arg;
	pThread->Run();
	return 0;
}

sThread::sThread() : m_bStarted(false)
{

}
sThread::~sThread()
{
	CloseHandle((HANDLE)m_hThread);
}