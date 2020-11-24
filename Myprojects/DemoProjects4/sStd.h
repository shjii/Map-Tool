#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <set>
#include <algorithm>
#include <tchar.h>
#pragma comment (lib, "winmm.lib")

using namespace std;

struct TInputActionMap
{
	DWORD   bWKey;
	DWORD   bSKey;
	DWORD   bAKey;
	DWORD   bDKey;
	DWORD   bLeftClick;
	DWORD   bRightClick;
	DWORD   bMiddleClick;
	DWORD   bExit;
	DWORD   bSpace;
};
static TInputActionMap g_KeyMap;

extern float g_fGameTimer;
extern float g_fSecondPerFrame;
extern RECT g_rtClient;
extern HWND g_hWnd;
extern HDC g_hScreenDC;
extern HDC g_hOffScreenDC;




template <class T>
class GetSing
{
public:
	static T& GetInstance()
	{
		static T instance;
		return instance;
	}
};