#include "sNetworkThread.h"

void main()
{
	HANDLE hExectuemutex = CreateMutex(NULL, FALSE, L"OnceMutex");
	if (GetLastError() == ERROR_ALIAS_EXISTS)
	{
		CloseHandle(hExectuemutex);
		return;
	}
	sNetworkThread net;
	if (net.InitNetwork("132.168.50.82", 10000) == false)
	{
		return;
	}
	net.Run();
	net.DeleteNetwork();
	CloseHandle(hExectuemutex);
}