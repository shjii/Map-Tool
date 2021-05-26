#pragma once
#include "KStd.h"
#include <mutex>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
struct KMsg
{
	RECT	rt;
	wstring msg;
	DWORD   dwColor;
	KMsg() {}
	KMsg(RECT rt, wstring szMsg, DWORD dwColor)
	{
		this->rt = rt;
		msg = szMsg;
		this->dwColor = dwColor;
	}
};
class KDxWrite : public KSingleton<KDxWrite>
{
public:
	ID2D1Factory*		m_pd2dFactory;
	ID2D1RenderTarget*  m_pd2dRT;
	IDWriteFactory*		m_pd2dWrite;
	IDWriteTextFormat*  m_pTextFormat;
	ID2D1SolidColorBrush*	m_pBrush = nullptr;
	UINT				m_iWidth;
	UINT				m_iHeight;
	float				m_fdpiX;
	float				m_fdpiY;
	std::vector<KMsg>   m_TextList;
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
	virtual bool Set(HWND hWnd,
		int iWidth, int iHeight,
		IDXGISurface1* pSurface);
	void ResizeDevice(int iWidth, int iHeight,
		IDXGISurface1* pSurface);
	HRESULT CreateIndependentResource();
	HRESULT CreateDependentResource(int iWidth, int iHeight,
		IDXGISurface1* pSurface);
	void DeleteDependentResource();
public:
	std::mutex      m_hMutex;
	virtual void   push(KMsg& msg);
	virtual void   Draw(POINT pos, wstring msg, DWORD color = RGB(255, 0, 0));
	virtual void   Draw(int x, int y, wstring msg, DWORD color = RGB(255, 0, 0));
public:
	KDxWrite();
	virtual ~KDxWrite();
};
#define g_dxWrite KDxWrite::GetInstance()