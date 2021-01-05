#include "sLoginServer.h"
#include "sIocp.h"
bool sLoginServer::Init()
{
	I_Iocp.Init();
	m_Acceptor.InitNetwork("192.168.50.82",10000);
	m_Acceptor.CreateThread();
	m_bRun = true;
	return false;
}
bool sLoginServer::Run()
{
	while (m_bRun)
	{
		Sleep(10);
	}
	WaitForSingleObject((HANDLE)m_Acceptor.m_hThread, INFINITE);
	return false;
}
bool sLoginServer::Release()
{
	return false;
}
sLoginServer::sLoginServer() : m_bRun(false)
{

}
sLoginServer::~sLoginServer()
{
	I_Iocp.Release();
}