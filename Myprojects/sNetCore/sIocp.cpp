#include "sIocp.h"
#include "sSessionMgr.h"
DWORD WINAPI WorkerThread(LPVOID param)
{
	sIocp* iocp = (sIocp*)param;
	BOOL bReturn;
	DWORD dwTransfor;
	ULONG_PTR keyValue;
	OVERLAPPED2* pOV;
	while (1)
	{
		//	if (WaitForSingleObject(g_hKillEvent, 1) == WAIT_OBJECT_0)
		//	{
		//		break;
		//	}
		//	if (WaitForSingleObject(g_hEventEOF, 1) == WAIT_OBJECT_0)
		//	{
		//		if (g_LargeRead.QuadPart <= g_LargeWrite.QuadPart)
		//		{
		//			::SetEvent(g_hKillEvent);
		//			continue;
		//		}
		//	}
			// 비동기 작업 결과를 iocp큐에서 확인
		bReturn = ::GetQueuedCompletionStatus(iocp->m_hIOCP,
			&dwTransfor,
			&keyValue,
			(LPOVERLAPPED*)&pOV, 1);// INFINITE);

		sNetUser* pUser = (sNetUser*)keyValue;
		if (pOV != nullptr && pOV->iType == OVERLAPPED2::MODE_EXIT)
		{
			I_Session.DelUser(pUser);
			continue;
		}
		// 스레드 깨운다
		if (bReturn == TRUE)
		{
			// 정상종료
			if (dwTransfor == 0)
			{
				pOV->iType = OVERLAPPED2::MODE_EXIT;
				PostQueuedCompletionStatus(iocp->m_hIOCP, 0,
					keyValue, (LPOVERLAPPED)pOV);
			}

			if (pOV->iType == OVERLAPPED2::MODE_RECV)
			{
				//// 로드완료
				if (pUser->DispatchRead(dwTransfor, pOV) == false)
				{
					pOV->iType = OVERLAPPED2::MODE_EXIT;
					PostQueuedCompletionStatus(iocp->m_hIOCP, 0,
						keyValue, (LPOVERLAPPED)pOV);
				}
			}
			if (pOV->iType == OVERLAPPED2::MODE_SEND)
			{
				if (iocp->DispatchWrite(dwTransfor, pOV) == false)
				{
					pOV->iType = OVERLAPPED2::MODE_EXIT;
					PostQueuedCompletionStatus(iocp->m_hIOCP, 0,
						keyValue, (LPOVERLAPPED)pOV);
				}
			}
		}
		else
		{
			DWORD dwError = WSAGetLastError();
			if (dwError == WAIT_TIMEOUT) { continue; }
			closesocket(pUser->m_Sock);
		}
	}
	return 0;
}
bool sIocp::DispatchRead(DWORD dwTrans, OVERLAPPED2 * ov)
{
	return true;
}

bool sIocp::DispatchWrite(DWORD dwTrans, OVERLAPPED2 * ov)
{
	return true;
}

bool sIocp::Init()
{
	m_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0,0,0);
	for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
	{
		m_hWorkThread[iThread] = ::CreateThread(0,0, WorkerThread, this, 0, &m_dwThreadID[iThread]);
	}
	return false;
}

bool sIocp::Run()
{
	return false;
}

bool sIocp::Release()
{
	CloseHandle(m_hIOCP);
	for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
	{
		CloseHandle(m_hWorkThread[iThread]);
	}
	return false;
}

void sIocp::SetBind(SOCKET sock, ULONG_PTR key)
{
	m_hIOCP = ::CreateIoCompletionPort(
		(HANDLE)sock, m_hIOCP, key, 0);
}

sIocp::sIocp()
{
}

sIocp::~sIocp()
{
}
