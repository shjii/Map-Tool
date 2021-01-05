#include "sLoginServer.h"

void main()
{
	sLoginServer server;
	server.Init();
	server.Run();
	server.Release();
}