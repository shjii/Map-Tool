#include "sLoginServer.h"
//*�Ӽ� : C/C++ -> �ڵ���� -> ��Ÿ�Ӷ��̺귯��(���߽�����/MT) 
void main()
{
	sLoginServer server;
	server.Init();
	server.Run();
	server.Release();
}