#include "TCore.h"
#include "TObjectMgr.h"

HRESULT TCore::DeleteDXResource()
{
	g_dxWrite.DeleteDependentResource();
	return S_OK;
}
HRESULT TCore::CreateDXResource(UINT w, UINT h)
{
	IDXGISurface1* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface),
		(LPVOID*)&pBackBuffer);
	g_dxWrite.ResizeDevice(w, h, pBackBuffer);
	if (pBackBuffer) pBackBuffer->Release();
	return S_OK;
}
bool	TCore::PreInit()
{
	return true;
};
bool	TCore::Init()
{
	return true;
};
bool	TCore::PostInit()
{

	return true;
};
bool TCore::GameInit()
{
	PreInit();
	m_bGameRun = true;
	if (TDevice::Init() == false)
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

	m_Camera.CreateViewMatrix({ 0,100,-10 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_Camera.CreateProjMatrix(1, 1000, TBASIS_PI / 4.0f, fAspect);
	m_Camera.Init();
	m_pMainCamera = &m_Camera;

	if (!m_LineShape.Create(g_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/tileA.jpg"))
	{
		return false;
	}
	g_TextMgr.Init();
	Init();
	PostInit();
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	return true;
}
bool TCore::GameRelease()
{
	m_LineShape.Release();
	Release();
	g_Timer.Release();
	g_Input.Release();
	g_SoundMgr.Release();
	g_ObjectMgr.Release();
	g_dxWrite.Release();
	TDevice::Release();
	return true;
}
bool	TCore::GameFrame()
{
	PreFrame();
	g_Timer.Frame();
	g_Input.Frame();
	g_SoundMgr.Frame();
	Frame();
	g_ObjectMgr.Frame();
	CameraFrame();
	PostFrame();
	return true;
}
void    TCore::CameraFrame()
{
	if (g_Input.GetKey('W') == KEY_HOLD)
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
	m_pMainCamera->Frame();
}
bool	TCore::PreRender()
{
	TDevice::PreRender();
	return true;
}
bool	TCore::PostRender()
{
	m_LineShape.SetMatrix(NULL, &m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_LineShape.Draw(g_pImmediateContext,
		Vector3(0, 0, 0), Vector3(100, 0, 0), Vector4(1, 0, 0, 1));
	m_LineShape.Draw(g_pImmediateContext,
		Vector3(0, 0, 0), Vector3(0, 100, 0), Vector4(0, 1, 0, 1));
	m_LineShape.Draw(g_pImmediateContext,
		Vector3(0, 0, 0), Vector3(0, 0, 100), Vector4(0, 0, 1, 1));

	g_Timer.Render();
	g_Input.Render();
	g_SoundMgr.Render();

	g_dxWrite.Render();
	g_dxWrite.Draw(0, 0, g_Timer.m_szBuffer);
	TDevice::PostRender();
	return true;
}
bool	TCore::GameRender()
{
	if (PreRender() == false) return false;
	if (Render() == false) return false;
	if (PostRender() == false) return false;
	return true;
}
bool	TCore::GameRun()
{
	if (GameFrame() == false) return false;
	if (GameRender() == false) return false;
	return true;
}
bool TCore::Run()
{
	//HeapEnableTerminateOnCorruption 옵션을 설정하면 손상된 힙을 이용하는 보안 악용에 대한 애플리케이션의 노출을 줄일 수 있으므로 강력하게 권장.
	//HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	HRESULT hr = S_OK;
	//CoInitializeEx 는 COM 라이브러리를 사용하는 각 스레드에 대해 한 번 이상 호출해야  한다.
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
TCore::TCore()
{
	m_pMainCamera = nullptr;
}
TCore::~TCore()
{

}