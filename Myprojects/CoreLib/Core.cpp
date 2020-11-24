#include "Core.h"

bool Core::Run()
{
	m_bGameRun = true;
	GameInit();
	while (m_bGameRun)
	{
		// 메세지 큐에 있는 메세지 반환
		if (msgProcess() == false)
		{
			m_bGameRun = false;
		}
		else
		{
			GameRun();
		}
	}
	GameRelease();
	return false;
}


bool Core::GameRun()
{
	Frame();
	Render();
	//게임 로직이 돌아간다.
//출력창에 출력하느 ㄴ함수 배열을 받아서 출력
	std::wstring msg;
	msg = L"\nSJH";
	//msg += std::to_wstring(iCount++);
	OutputDebugString(msg.c_str());
	Sleep(10);// ms
	return true;
}

bool Core::GameInit()
{
	Init();
	return true;
}
bool Core::GameRelease()
{
	Release();
	return true;
}