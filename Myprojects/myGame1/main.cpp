#include <Windows.h>

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

BOOL InitInstance( const WCHAR* szWindowClass, const WCHAR* szTitle,
					HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd = CreateWindowW(
		szWindowClass,
		szTitle,
		WS_OVERLAPPED | 
			WS_CAPTION | 
			WS_SYSMENU | 
			WS_THICKFRAME | 
			WS_MINIMIZEBOX | 
			WS_MAXIMIZEBOX,
		300, 300,
		800, 600,
		nullptr, nullptr, 
		hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}
	ShowWindow(hWnd, SW_SHOWNORMAL);
	//UpdateWindow(hWnd);
	return TRUE;
}

ATOM MyRegisterClass(
	HINSTANCE hInstance, const WCHAR* szClassName, int iBrush = 0)
{
	WNDCLASSEXW wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEXW));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hbrBackground = (HBRUSH)GetStockObject(iBrush);
	// 운영체제-> winproc(메세지큐) ->(메시지) 윈도우
	// 윈도우 -> 윈도우지원함수 -> 운영체제
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = szClassName;
	WORD ret = RegisterClassExW(&wcex);
	return ret;
}

int APIENTRY wWinMain(
	HINSTANCE hInstance, // 고유한 번호를 가진 구조체
	HINSTANCE hPrevInstance, // 
	LPWSTR    lpCmdLine,
	int       nCmdShow)// 어떻게 보이는지 ?(창 크기)
{
	//윈도우 등록
	MyRegisterClass(hInstance, L"SJH", GRAY_BRUSH);

	//윈도우 생성
	 // 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(L"SJH",L"SJH",hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG msg;

	// 기본 메시지 루프입니다:
	while (1)
	{
		// 메세지 큐에 있는 메세지 반환
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); // 키 번역
			DispatchMessage(&msg);  // 프로시져 전달
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			//게임 로직이 돌아간다.
		}

	}

	MessageBox(NULL, L"종료", L"sample", MB_OK);
}