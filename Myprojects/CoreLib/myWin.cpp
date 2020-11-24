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
	//1 윈도우 클래스 객체 등록
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = L"가나다";
	if (RegisterClassEx(&wc) == 0)
	{
		return false;
	}

	//2 윈도우 생성
	m_hwnd = CreateWindowEx(
							WS_EX_APPWINDOW,
							L"가나다",
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
//3 윈도우 메세지 루프

bool myWin::msgProcess()
{
	ZeroMemory(&msg, sizeof(msg));
	// 메세지 큐에 있는 메세지 반환
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg); // 키 번역
		DispatchMessage(&msg);  // 프로시져 전달
		if (msg.message == WM_QUIT)
		{
			return false;
		}
	}
	return true;
}
