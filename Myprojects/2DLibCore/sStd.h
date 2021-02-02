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
#include <assert.h>
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "sCoreLib.lib")
#pragma comment (lib, "fmod_vc.lib")
#pragma comment( lib, "Msimg32.lib")
using namespace std;

struct sPoint
{
	float x;
	float y;
	sPoint() = default;
	sPoint(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	sPoint operator - (sPoint& p)
	{
		sPoint ret;
		ret.x = x - p.x;
		ret.y = y - p.y;
		return ret;
	}
	sPoint operator + (sPoint& p)
	{
		sPoint ret;
		ret.x = x = p.x;
		ret.y = y = p.y;
		return ret;
	}
	sPoint operator * (float fValue)
	{
		sPoint ret;
		ret.x = x * fValue;
		ret.y = y * fValue;
		return ret;
	}
	sPoint operator + (sPoint p)
	{
		sPoint ret;
		ret.x = x + p.x;
		ret.y = y + p.y;
		return ret;
	}
	float Length()
	{
		return sqrt(x*x + y * y);
	}
	sPoint Normalize()
	{
		float fD = Length();
		x /= fD;
		y /= fD;
		return sPoint(x, y);
	}
};

struct TInputActionMap
{
	DWORD   bWKey;
	DWORD   bSKey;
	DWORD   bAKey;
	DWORD   bDKey;
	DWORD   bZKey;
	DWORD   bXKey;
	DWORD   bLeftClick;
	DWORD   bRightClick;
	DWORD   bMiddleClick;
	DWORD   bExit;
	DWORD   bSpace;
};
extern TInputActionMap g_KeyMap;

extern float g_fGameTimer;
extern float g_fSecondPerFrame;
extern RECT g_rtClient;
extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
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