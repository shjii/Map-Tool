#include "Core.h"

bool Core::Run()
{
	m_bGameRun = true;
	GameInit();
	while (m_bGameRun)
	{
		// �޼��� ť�� �ִ� �޼��� ��ȯ
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
	//���� ������ ���ư���.
//���â�� ����ϴ� ���Լ� �迭�� �޾Ƽ� ���
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