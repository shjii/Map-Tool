#pragma once
#include "sNetStd.h"
#include "sNetUser.h"
#define MAX_WORKER_THREAD 3
class sIocp : public sNetSingleton<sIocp>
{
private:
	friend class sNetSingleton<sIocp>;
public:
	HANDLE m_hIOCP;
	HANDLE	 m_hWorkThread[MAX_WORKER_THREAD];
	DWORD	 m_dwThreadID[MAX_WORKER_THREAD];
	bool DispatchRead(DWORD dwTrans, OVERLAPPED2* ov);
	bool DispatchWrite(DWORD dwTrans, OVERLAPPED2* ov);
public:
	bool Init();
	bool Run();
	bool Release();
	void SetBind(SOCKET sock, ULONG_PTR key);
public:
	sIocp();
	virtual ~sIocp();
};
#define I_Iocp sIocp::GetInstance()
