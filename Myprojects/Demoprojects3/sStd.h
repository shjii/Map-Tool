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
using namespace std;

extern float g_fGameTimer;
extern float g_fSecondPerFrame;

extern HWND g_hWnd;


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