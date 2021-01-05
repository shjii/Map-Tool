#include "sNetworkThread.h"
DWORD WINAPI AcceptThread(LPVOID arg)
{
	sNetwork* net = (sNetwork*)arg;
	while (1)
	{
		if (!net->Accept()) break;

		Sleep(1);
	}
	closesocket(net->m_Sock);
	return 0;
}
DWORD WINAPI ReceveThread(LPVOID arg)
{
	sNetwork* net = (sNetwork*)arg;
	while (1)
	{
		if (!net->RecvUserList()) break;

		Sleep(1);
	}
	closesocket(net->m_Sock);
	return 0;
}
DWORD WINAPI ProcessThread(LPVOID arg)
{
	sNetwork* net = (sNetwork*)arg;
	while (1)
	{
		if (!net->Process()) break;

		Sleep(1);
	}
	closesocket(net->m_Sock);
	return 0;
}
bool sNetworkThread::InitSocket(string ip, int port)
{
	if (!sNetwork::InitSocket(ip,port))
	{
		return false;
	}
	DWORD dwID;
	m_hAccept = CreateThread(0,0, AcceptThread,(LPVOID)this,0,&dwID);
	m_hRead = CreateThread(0, 0, ReceveThread, (LPVOID)this, 0, &dwID);
	m_hProcess = CreateThread(0, 0, ProcessThread, (LPVOID)this, 0, &dwID);

	m_hMutex = CreateMutex(NULL, FALSE, L"CharProcess");
}
bool sNetworkThread::Run()
{
	while (1)
	{
		system("cls");
		printf("\nUserCnt:%d", m_Userlist.size());
		Sleep(1);
	}
}

