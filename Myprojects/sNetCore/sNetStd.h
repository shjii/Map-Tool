#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h> // windows.h ���� �켱 �����Ѵ�.
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <atlconv.h> // A2W
#include <process.h> //_beginthreadex
#include <map>
#include "sProtocol.h"

#pragma comment (lib, "ws2_32.lib")
#if NDEBUG
#pragma comment (lib, "TNetCore_R.lib")
#else
#pragma comment (lib, "TNetCore_D.lib")
#endif
using namespace std;

typedef std::basic_string<TCHAR>		T_STR;
typedef std::basic_string<CHAR>			C_STR;
typedef std::vector<T_STR>				T_STR_VECTOR;


static std::wstring to_mw(const std::string& _src)
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
	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = WideCharToMultiByte(CP_ACP, 0,
		data, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0,
		data, -1,  //  �ҽ�
		retData, iLength, // ���
		NULL, NULL);
	return retData;
}
static bool GetWtM(WCHAR* src, char* pDest)
{
	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = WideCharToMultiByte(CP_ACP, 0,
		src, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0,
		src, -1,  //  �ҽ�
		pDest, iLength, // ���
		NULL, NULL);
	if (iRet == 0) return false;
	return true;
}
static WCHAR* GetMtW(char* data)
{
	WCHAR retData[4096] = { 0 };
	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0,
		data, -1,  //  �ҽ�
		retData, iLength); // ���
	return retData;
}
static bool GetMtW(char* pSrc, WCHAR* pDest)
{
	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = MultiByteToWideChar(CP_ACP, 0,
		pSrc, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0,
		pSrc, -1,  //  �ҽ�
		pDest, iLength); // ���		
	if (iRet == 0) return false;
	return true;
}
static void PRINT(char* fmt, ...) // ����������
{
	va_list arg;
	va_start(arg, fmt);
	char buf[256] = { 0, };
	vsprintf_s(buf, fmt, arg);
	printf("\n=====> %s", buf);
	va_end(arg);
}

static void Error(const CHAR* msg = 0, const char* IpData = 0)
{
	LPVOID* IpMsg = 0;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(CHAR*)&IpMsg, 0, NULL);
	string szBuffer = (IpData != nullptr) ? IpData : "";
	szBuffer += "\n";
	szBuffer += (CHAR*)IpMsg;

	MessageBoxA(NULL, szBuffer.c_str(), msg, MB_ICONERROR);
	LocalFree(IpMsg);
}
static void Check(int iRet, int line)
{
	if (iRet == SOCKET_ERROR)
	{
		CHAR szBuffer[256] = { 0 , };
		sprintf_s(szBuffer, "%s[%d]", __FILE__, line);
		Error("ERROR", szBuffer);
		exit(1);
	}
}
template<class T>
class sNetSingleton
{
public:
	static T& GetInstance()
	{
		static T theSingle;
		return theSingle;
	}
};