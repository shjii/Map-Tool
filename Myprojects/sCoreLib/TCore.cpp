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
		g_dxWrite.ResizeDevice(w,h, pBackBuffer);
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
		if (TDevice::Init()==false)
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

		Init();
	PostInit();
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	return true;
}
bool TCore::GameRelease()
{	
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
	PostFrame();
	return true;
}
bool	TCore::PreRender()
{
	TDevice::PreRender();
	return true;
}
bool	TCore::PostRender()
{
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
	if (GameFrame()==false) return false;
	if (GameRender() == false) return false;
	return true;
}
bool TCore::Run()
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
			if (GameRun()==false)
			{
				break;
			}
		}
	}
	GameRelease();
	CoUninitialize();
	return true;
}