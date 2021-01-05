#include <iostream>
#include <windows.h>
#include <tchar.h>
char* GetWtM(WCHAR* data)
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
TCHAR* g_pFileBuffer = nullptr;
OVERLAPPED g_readOV;
OVERLAPPED g_writeOV;
void main()
{
	HANDLE hReadFile = CreateFile(L"sql.ISO",
		GENERIC_READ, 0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	//FILE_ATTRIBUTE_NORMAL , NULL);

	LARGE_INTEGER filesize;
	if (hReadFile != NULL)
	{
		GetFileSizeEx(hReadFile, &filesize);
		g_pFileBuffer = new TCHAR[filesize.QuadPart];

		DWORD dwRead;
		LONGLONG dwLength = filesize.QuadPart;

		// 비동기 입출력
		// 야! 읽어 -> 반환 -> 그리고 다 읽으면 통지해죠!
		BOOL ret = ReadFile(hReadFile, g_pFileBuffer, dwLength, &dwRead,
			&g_readOV);
		BOOL bPending = FALSE;
		if (ret == FALSE)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				_tcprintf(L"\n읽기 중!");
				bPending = TRUE;
			}
		}
		if (ret == TRUE)
		{
			_tcprintf(L"\n읽기 완료!");
		}
		DWORD dwTrans;
		while (bPending)
		{
			ret = GetOverlappedResult(hReadFile, &g_readOV, &dwTrans, FALSE);
			if (ret == TRUE)
			{
				bPending = FALSE;
			}
			else
			{
				if (GetLastError() == ERROR_IO_INCOMPLETE)
				{
					std::cout << " Loading..." << "ERROR_IO_INCOMPLETE(996)" << std::endl;
				}
				else
				{
					std::cout << " Loading Error!" << GetLastError() << std::endl;
					break;
				}
			}
			Sleep(100);
		}
	}
	CloseHandle(hReadFile);

	HANDLE hWriteFile = CreateFile(L"COPY3.iso", GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (hWriteFile != NULL)
	{
		DWORD dwWritten;
		DWORD dwLength = filesize.LowPart;
		BOOL ret = WriteFile(hWriteFile, g_pFileBuffer, dwLength, &dwWritten,
			&g_writeOV);
		BOOL bPending = FALSE;
		if (ret == FALSE)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				_tcprintf(L"\n쓰기 중!");
				bPending = TRUE;
			}
		}
		if (ret == TRUE)
		{
			_tcprintf(L"\n쓰기 완료!");
		}
		DWORD dwTrans;
		while (bPending)
		{
			ret = GetOverlappedResult(hReadFile, &g_readOV, &dwTrans, FALSE);
			if (ret == TRUE)
			{
				bPending = FALSE;
			}
			else
			{
				if (GetLastError() == ERROR_IO_INCOMPLETE)
				{
					std::cout << " Loading..." << "ERROR_IO_INCOMPLETE(996)" << std::endl;
				}
				else
				{
					std::cout << " Loading Error!" << GetLastError() << std::endl;
					break;
				}
			}
			Sleep(100);
		}
	}
	CloseHandle(hWriteFile);


}