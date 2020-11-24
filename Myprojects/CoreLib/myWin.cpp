#include "myWin.h"

myWin::myWin()
{

}
myWin::~myWin()
{

}

LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0); // WM_QUIT
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool myWin::SetWindow(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	//1 ������ Ŭ���� ��ü ���
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = L"������";
	if (RegisterClassEx(&wc) == 0)
	{
		return false;
	}

	//2 ������ ����
	m_hwnd = CreateWindowEx(
							WS_EX_APPWINDOW,
							L"������",
							L"MyGame2",
							WS_OVERLAPPEDWINDOW,
							300, 300,
							800, 600,
							nullptr, nullptr,
							m_hInstance, nullptr);
	if (m_hwnd == NULL)
	{
		return false;
	}
	ShowWindow(m_hwnd, SW_SHOW);
	


	return true;
}
//3 ������ �޼��� ����

bool myWin::msgProcess()
{
	ZeroMemory(&msg, sizeof(msg));
	// �޼��� ť�� �ִ� �޼��� ��ȯ
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg); // Ű ����
		DispatchMessage(&msg);  // ���ν��� ����
		if (msg.message == WM_QUIT)
		{
			return false;
		}
	}
	return true;
}
