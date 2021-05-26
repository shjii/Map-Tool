#include "KCore.h"

HRESULT KCore::DeleteDXResource()
{
	g_dxWrite.DeleteDependentResource();
	return S_OK;
}
HRESULT KCore::CreateDXResource(UINT w, UINT h)
{
	IDXGISurface1* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface),
		(LPVOID*)&pBackBuffer);
	g_dxWrite.ResizeDevice(w, h, pBackBuffer);
	if (pBackBuffer) pBackBuffer->Release();
	return S_OK;
}
bool	KCore::PreInit()
{
	return true;
};
bool	KCore::Init()
{
	return true;
};
bool	KCore::PostInit()
{

	return true;
};
bool KCore::GameInit()
{
	PreInit();
	m_bGameRun = true;
	if (KDevice::Init() == false)
	{
		return false;
	}
	SetMode(m_bFullScreen);

	g_Timer.Init();
	g_Input.Init();
	g_SoundMgr.Init();

	IDXGISurface1* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface),
		(LPVOID*)&pBackBuffer);
	g_dxWrite.Set(m_hWnd,
		g_rtClient.right,
		g_rtClient.bottom, pBackBuffer);
	if (pBackBuffer) pBackBuffer->Release();

	//m_Camera.CreateViewMatrix({ 0,10,-10 }, { 0,0,0 });
	//float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	//m_Camera.CreateProjMatrix(1, 100000, TBASIS_PI / 4.0f, fAspect);
	//m_Camera.Init();
	//m_pMainCamera = &m_Camera;

	g_TextureMgr.Init();
	Init();
	PostInit();
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	return true;
}
bool KCore::GameRelease()
{
	Release();
	g_Timer.Release();
	g_Input.Release();
	g_SoundMgr.Release();
	//g_ObjectMgr.Release();
	g_dxWrite.Release();
	KDevice::Release();
	return true;
}
bool	KCore::GameFrame()
{
	PreFrame();
	g_Timer.Frame();
	g_Input.Frame();
	g_SoundMgr.Frame();
	Frame();
	//g_ObjectMgr.Frame();
	CameraFrame();
	PostFrame();
	return true;
}
void    KCore::CameraFrame()
{
	/*if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_pMainCamera->FrontMovement(1.0f);
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_pMainCamera->FrontMovement(-1.0f);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_pMainCamera->RightMovement(-1.0f);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_pMainCamera->RightMovement(1.0f);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_pMainCamera->UpMovement(1.0f);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_pMainCamera->UpMovement(-1.0f);
	}
	m_pMainCamera->Frame();*/
}
bool	KCore::PreRender()
{
	KDevice::PreRender();
	return true;
}
bool	KCore::PostRender()
{
	g_Timer.Render();
	g_Input.Render();
	g_SoundMgr.Render();

	g_dxWrite.Render();
	g_dxWrite.Draw(0, 0, g_Timer.m_szBuffer);
	KDevice::PostRender();
	return true;
}
bool	KCore::GameRender()
{
	if (PreRender() == false) return false;
	if (Render() == false) return false;
	if (PostRender() == false) return false;
	return true;
}
bool	KCore::GameRun()
{
	if (GameFrame() == false) return false;
	if (GameRender() == false) return false;
	return true;
}
bool KCore::Run()
{
	//HeapEnableTerminateOnCorruption �ɼ��� �����ϸ� �ջ�� ���� �̿��ϴ� ���� �ǿ뿡 ���� ���ø����̼��� ������ ���� �� �����Ƿ� �����ϰ� ����.
	//HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	HRESULT hr = S_OK;
	//CoInitializeEx �� COM ���̺귯���� ����ϴ� �� �����忡 ���� �� �� �̻� ȣ���ؾ�  �Ѵ�.
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	GameInit();
	while (m_bGameRun)
	{
		if (MsgProcess() == false)
		{
			m_bGameRun = false;
		}
		else
		{
			if (GameRun() == false)
			{
				break;
			}
		}
	}
	GameRelease();
	CoUninitialize();
	return true;
}
bool	KCore::InitTool(HWND hwnd, HINSTANCE hlnstance)
{
	HRESULT hr = S_OK;
	g_hWnd = m_hWnd = hwnd;
	g_hInstance = m_hInstance = hlnstance;

	GetClientRect(m_hWnd, &m_rtClient);
	g_rtClient = m_rtClient;
	GetWindowRect(m_hWnd, &m_rtWindow);
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	GameInit();
	return true;
}
bool	KCore::ReleaseTool()
{
	GameRelease();
	CoUninitialize();
	return true;
}
bool	KCore::ToolRun()
{
	if (GameFrame() == false) return false;
	if (GameRender() == false) return false;
	return true;
}
KCore::KCore()
{
	//m_pMainCamera = nullptr;
}
KCore::~KCore()
{

}