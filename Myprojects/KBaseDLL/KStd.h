#pragma once
#include <winsock2.h>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <algorithm>
#include <functional>
#include <assert.h>
#include <atlconv.h> // A2W
#include <iostream>
#include <memory>
#include <process.h> //_beginthreadex
#include <d3dcompiler.h>
#include <d3d11.h>
#include <wrl.h>
#include "SimpleMath.h"
//#include "KMath.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment( lib, "Winmm.lib")
#pragma comment (lib, "ws2_32.lib")
#pragma comment( lib, "fmod_vc.lib")
#pragma warning (disable:4244) 
using namespace DirectX;
using namespace SimpleMath;
using namespace Microsoft::WRL;
using namespace std;

#ifdef KBASEDLL_EXPORTS
#define MATHFUNCSDLL_API __declspec(dllexport)
#else
#define MATHFUNCSDLL_API __declspec(dllimport)
#endif
enum K_POSITION
{
	P_BACK = 0,    // 뒤
	P_FRONT,   // Point is positioned in front of plane	
	P_ONPLANE, // Point is positioned on plane
	P_SPANNING // Point is spanning plane
};

namespace KBASIS_CORE_LIB
{
	typedef std::basic_string<TCHAR>		T_STR;
	typedef std::basic_string<CHAR>			C_STR;
	typedef std::vector<T_STR>				T_STR_VECTOR;

	template<class T>
	class  KSingleton
	{
	public:
		static T& GetInstance()
		{
			static T theSingle;
			return theSingle;
		}
	};
	template <typename T>
	inline void SafeRelease(T *&p)
	{
		if (NULL != p)
		{
			p->Release();
			p = NULL;
		}
	}

#ifdef __cplusplus
	extern "C" 
	{
#endif
		static wstring to_mw(const std::string& _src)
		{
			USES_CONVERSION;
			return std::wstring(A2W(_src.c_str()));
		};

		static std::string to_wm(const std::wstring& _src)
		{
			USES_CONVERSION;
			return std::string(W2A(_src.c_str()));
		};

		static char* GetWtM(WCHAR* data)
		{
			char retData[4096] = { 0 };
			// 변형되는 문자열의 크기가 반환된다.
			int iLength = WideCharToMultiByte(CP_ACP, 0,
				data, -1, 0, 0, NULL, NULL);
			int iRet = WideCharToMultiByte(CP_ACP, 0,
				data, -1,  //  소스
				retData, iLength, // 대상
				NULL, NULL);
			return retData;
		}
		static bool GetWtM(WCHAR* src, char* pDest)
		{
			// 변형되는 문자열의 크기가 반환된다.
			int iLength = WideCharToMultiByte(CP_ACP, 0,
				src, -1, 0, 0, NULL, NULL);
			int iRet = WideCharToMultiByte(CP_ACP, 0,
				src, -1,  //  소스
				pDest, iLength, // 대상
				NULL, NULL);
			if (iRet == 0) return false;
			return true;
		}

		static void PRINT(char* fmt, ...) // 나열연산자
		{
			va_list arg;
			va_start(arg, fmt);
			char buf[256] = { 0, };
			vsprintf_s(buf, fmt, arg);
			printf("\n=====> %s", buf);
			va_end(arg);
		}
		static void Error(const CHAR* msg = 0, const char* lpData = 0)
		{
			LPVOID* lpMsg = 0;
			FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, WSAGetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(CHAR*)&lpMsg, 0, NULL);

			std::string szBuffer = (lpData != nullptr) ? lpData : "";
			szBuffer += "\n";
			szBuffer += (CHAR*)lpMsg;

			MessageBoxA(NULL, szBuffer.c_str(), msg, MB_ICONERROR);
			LocalFree(lpMsg);
		}
		static void Check(int iRet, int line)
		{
			if (iRet == SOCKET_ERROR)
			{
				CHAR	szBuffer[256] = { 0, };
				sprintf_s(szBuffer,
					"%s[%d]",
					__FILE__,
					line);
				Error("ERROR", szBuffer);
				exit(1);
			}
		}
		inline void OutputDebug(const char* format, ...)
		{
#ifdef _DEBUG
			va_list args;
			va_start(args, format);

			char buff[1024] = {};
			vsprintf_s(buff, format, args);
			OutputDebugStringA(buff);
			va_end(args);
#else
			UNREFERENCED_PARAMETER(format);
#endif
		}

		struct KKeyMap
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


		extern ID3D11Device*  g_pd3dDevice;
		extern ID3D11DeviceContext* g_pImmediateContext;
		extern KKeyMap  g_KeyMap;
		extern float	g_fGameTimer;
		extern float    g_fSecondPerFrame;
		extern HWND     g_hWnd;
		extern HINSTANCE     g_hInstance;
		extern RECT		g_rtClient;
		extern bool		g_bActive;

#ifdef __cplusplus
	}
#endif

}

/////////////////////////////////////////  반환하지 않는다. ////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG) 
#ifndef H
//주의사항:매크로 함수안에서 매크로 함수(x가 함수로 해석)를 중복 실행 되지 않게 x=DX메소드만 사용해야 한다.
#define H(x){ hr = (x);if (FAILED(hr)){\
							LPWSTR output;\
							WCHAR buffer[256]={0,};\
							FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
							NULL,x,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
							wsprintf(buffer,L"File=%s\nLine=%d", _T(__FILE__), __LINE__);\
							MessageBox(NULL, buffer,output,MB_OK);}\
						}
#endif
#else
#ifndef H
#define H(x) (x)
#endif
#endif

/////////////////////////////////////////  실폐의 경우만 반환한다 ////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG) 
#ifndef H_RETURN
//주의사항:hr = (x); 이 있어야 매크로 함수안에서 매크로 함수(x가 함수로 해석)를 중복 실행하지 않는다.
#define H_RETURN(x){  hr = (x); if (FAILED(hr)){\
							LPWSTR output;\
							WCHAR buffer[256]={0,};\
							FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
							NULL,hr,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
							wsprintf(buffer,L"File=%s\nLine=%d", _T(__FILE__),__LINE__);\
							MessageBox(NULL, buffer,output,MB_OK); return hr;}\
						}
#endif
#else
#define H_RETURN(x){ hr = (x); if( FAILED(hr) ) { return hr; }}
#endif

/////////////////////////////////////////  무조건 반환한다 ////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG) 
#ifndef U_RETURN
//주의사항:hr = (x); 이 있어야 매크로 함수안에서 매크로 함수(x가 함수로 해석)를 중복 실행하지 않는다.
#define U_RETURN(x){  hr = (x);if (FAILED(hr)){\
							LPWSTR output;\
							WCHAR buffer[256]={0,};\
							FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
							NULL,x,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
							wsprintf(buffer,L"File=%s\nLine=%d", _T(__FILE__),__LINE__);\
							MessageBox(NULL, buffer,output,MB_OK); return hr;}\
							else { return hr;}\
						}
#endif
#else
#define U_RETURN(x){ hr = (x); return hr; }
#endif


#ifndef V
#define V(x)       { hr = (x); }
#endif
#ifndef V_RETURN
#define V_RETURN(x){ hr = (x); if( FAILED(hr) ) { return hr; } }
#endif
#ifndef T_RETURN
#define T_RETURN(x,ret){ hr = (x); if( FAILED(hr) ) { return ret; } }
#endif

//////////////////////////////////////////////
// 객체 및 배열 할당과 삭제 및 소멸 매크로
//////////////////////////////////////////////
#ifndef SAFE_ZERO
#define SAFE_ZERO(A)				{ A = 0; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef SAFE_DEL
#define SAFE_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 

#ifndef SAFE_NEW_ARRAY
#define SAFE_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef SAFE_NEW_CLEAR
#define SAFE_NEW_CLEAR( A, B )			{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef SAFE_NEW_ARRAY_CLEAR
#define SAFE_NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif

using namespace KBASIS_CORE_LIB;