#pragma once
#include "TBasisStd.h"
#include <d3d11.h>
#include <wrl.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#if NDEBUG
#pragma comment (lib, "SCoreLib_R.lib")
#else
#pragma comment (lib, "SCoreLib_D.lib")
#endif

using namespace std;

namespace TBASIS_CORE_LIB
{
	struct TKeyMap
	{
		bool   m_bFront; // up, w
		bool   m_bLeft;  // left, a
		bool   m_bRight; // right. d
		bool   m_bBack;  // down, s
		bool   m_bAttack;// enter, lbutton
		bool   m_bJump;  // space, rbutton
		bool   m_bHangul;
		bool   m_bCapsLock;
		bool   m_bNumLock;
	};

	extern TKeyMap  g_KeyMap;
	extern float	g_fGameTimer;
	extern float    g_fSecondPerFrame;
	extern HWND     g_hWnd;
	extern HINSTANCE     g_hInstance;
	extern RECT		g_rtClient;
	extern bool		g_bActive;
}

using namespace TBASIS_CORE_LIB;