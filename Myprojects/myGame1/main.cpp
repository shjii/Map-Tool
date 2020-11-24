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
	// �ü��-> winproc(�޼���ť) ->(�޽���) ������
	// ������ -> �����������Լ� -> �ü��
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = szClassName;
	WORD ret = RegisterClassExW(&wcex);
	return ret;
}

int APIENTRY wWinMain(
	HINSTANCE hInstance, // ������ ��ȣ�� ���� ����ü
	HINSTANCE hPrevInstance, // 
	LPWSTR    lpCmdLine,
	int       nCmdShow)// ��� ���̴��� ?(â ũ��)
{
	//������ ���
	MyRegisterClass(hInstance, L"SJH", GRAY_BRUSH);

	//������ ����
	 // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
	if (!InitInstance(L"SJH",L"SJH",hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG msg;

	// �⺻ �޽��� �����Դϴ�:
	while (1)
	{
		// �޼��� ť�� �ִ� �޼��� ��ȯ
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); // Ű ����
			DispatchMessage(&msg);  // ���ν��� ����
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			//���� ������ ���ư���.
		}

	}

	MessageBox(NULL, L"����", L"sample", MB_OK);
}