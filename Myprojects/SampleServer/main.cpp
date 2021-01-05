#include "sLoginServer.h"
//*속성 : C/C++ -> 코드생성 -> 런타임라이브러리(다중스레드/MT) 
void main()
{
	sLoginServer server;
	server.Init();
	server.Run();
	server.Release();
}