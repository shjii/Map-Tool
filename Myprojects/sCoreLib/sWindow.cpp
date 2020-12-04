#include "sWindow.h"

HINSTANCE g_hInstance;
HWND g_hWnd = 0;
RECT g_rtClient;
LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);// WM_QUIT
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool sWindow::MsgProcess()
{
	while(PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
		if (m_msg.message == WM_QUIT)
		{
			return false;
		}
	}
	return true;
}

bool sWindow::SetWindow(HINSTANCE hInstance)
{
	g_hInstance = hInstance;
	m_hInstance = hInstance;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = L"SJH Demo Projects";
	if (RegisterClassEx(&wc) == 0)
	{
		return false;
	}

	RECT rt = { 0,0, 1280, 720 };
	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		L"SJH Demo Projects",
		L"SJH Game",
		WS_OVERLAPPEDWINDOW,
		0, 0,
		rt.right - rt.left,
		rt.bottom - rt.top,
		nullptr, nullptr,
		m_hInstance, nullptr);

	if (m_hWnd == NULL)
	{
		return false;
	}

	g_hWnd = m_hWnd;
	GetClientRect(m_hWnd, &m_rtClient);
	g_rtClient = m_rtClient;

	ShowWindow(m_hWnd, SW_SHOW);
	
	return true;
}

void sWindow::MsgEvent(MSG msg)
{

}

sWindow::sWindow()
{
	ZeroMemory(&m_msg, sizeof(MSG));
}
sWindow::~sWindow()
{

}