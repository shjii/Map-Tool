#include "sBitmapManager.h"
bool sBitmapManager::SaveFile(HBITMAP hBitmap, const TCHAR* strSaveFile)
{
	BITMAP bitmap;
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);

	BITMAPINFOHEADER bitHeader;
	memset(&bitHeader, 0, sizeof(BITMAPCOREHEADER));
	bitHeader.biSize = sizeof(BITMAPCOREHEADER);
	bitHeader.biWidth = bitmap.bmWidth;
	bitHeader.biHeight = bitmap.bmHeight;
	bitHeader.biPlanes = 1;
	bitHeader.biBitCount = bitmap.bmBitsPixel;
	bitHeader.biCompression = BI_RGB;

	HDC hdc = GetDC(GetDesktopWindow());
	GetDIBits(hdc, hBitmap, 0, bitmap.bmHeight, NULL, (LPBITMAPINFO)&bitHeader, DIB_RGB_COLORS);

	LPBYTE IpBits = new BYTE[bitHeader.biSizeImage];
	GetDIBits(hdc, hBitmap, 0, bitmap.bmHeight, IpBits, (LPBITMAPINFO)&bitHeader, DIB_RGB_COLORS);
	ReleaseDC(GetDesktopWindow(), hdc);

	HANDLE hFile = CreateFile(strSaveFile, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		delete[] IpBits;
		return false;
	}
	// 파일 헤더 저장
	BITMAPFILEHEADER bmFileHeader;
	memset(&bmFileHeader, 0, sizeof(BITMAPFILEHEADER));
	bmFileHeader.bfType = ((WORD)('M' << 8) | 'B');//MAKEWORD(
	bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmFileHeader.bfSize = bmFileHeader.bfOffBits + bitHeader.biSizeImage;

	DWORD dwWritten;
	//파일헤더 저장
	WriteFile(hFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//정보헤더 저장
	WriteFile(hFile, &bitHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
	//데이터 저장
	WriteFile(hFile, IpBits, bitHeader.biSizeImage, &dwWritten, NULL);
	CloseHandle(hFile);

	delete[] IpBits;
	return true;
}
HBITMAP sBitmapManager::CreateDIB(LONG iWidth, LONG iHeight, LPVOID* pPixeIData)
{
	BITMAPINFO bmi;
	LPVOID pBits;

	HBITMAP hBitmap;
	memset(&bmi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biWidth = iWidth;
	bmi.bmiHeader.biHeight = iHeight;
	bmi.bmiHeader.biPlanes = 1;

	hBitmap = CreateDIBSection(g_hScreenDC, (BITMAPINFO*)&bmi,DIB_RGB_COLORS, &pBits, NULL, 0);

	for (int i = 0; i < iWidth; i++)
	{
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pBits)[i * iWidth];
		for (int j = 0; j < iHeight; j++)
		{
			pbSrcRGB[0] = 0;
			pbSrcRGB[1] = 0;
			pbSrcRGB[2] = 0;
			pbSrcRGB[3] = 255;
			pbSrcRGB += 4;
		}
	}

	if (pPixeIData != nullptr)
	{
		*pPixeIData = pBits;
	}
	return hBitmap;
}

sBitmap* sBitmapManager::Load(const TCHAR* filename)
{
	TCHAR drive[MAX_PATH] = { 0, };
	TCHAR dir[MAX_PATH] = { 0, };
	TCHAR name[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	_wsplitpath_s(filename, drive, dir, name, ext);

	wstring Dir = dir;
	wstring key;

	wstring loadfile;
	if (Dir.empty())
	{
		loadfile = szDefaultPath;
		loadfile += filename;
		key = filename;
	}
	else
	{
		loadfile = filename;
		key = name;
		key += ext;
	}

	sBitmap* pData = GetPtr(key);
	if (pData != nullptr)
	{
		return pData;
	}

	pData = new sBitmap;
	pData->Init();

	if (pData->Load(loadfile))
	{
		m_List.insert(make_pair(key, pData));
		return pData;
	}
	delete pData;
	return nullptr;
}

sBitmap* sBitmapManager::CreateLoad(LONG iWidth, LONG iHeight, const TCHAR* filename, int iBitCount)
{
	TCHAR drive[MAX_PATH] = { 0, };
	TCHAR dir[MAX_PATH] = { 0, };
	TCHAR name[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	_wsplitpath_s(filename, drive, dir, name, ext);

	wstring Dir = dir;
	wstring key;
	wstring loadfile;
	if (Dir.empty())
	{
		loadfile = szDefaultPath;
		loadfile += filename;
		key = filename;
	}
	else
	{
		loadfile = filename;
		key = name;
		key += ext;
	}

	sBitmap* pData = GetPtr(key);
	if (pData != nullptr)
	{
		return pData;
	}

	pData = new sBitmap;
	pData->Init();

	if (pData->CreateDIB(iWidth, iHeight, iBitCount))
	{
		m_List.insert(make_pair(key, pData));
		return pData;
	}
	delete pData;
	return nullptr;
}

sBitmap* sBitmapManager::GetPtr(wstring _name)
{
	m_iter = m_List.find(_name);
	if (m_iter == m_List.end())
	{
		return nullptr;
	}
		return (*m_iter).second;
}

bool sBitmapManager::Init()
{
	return true;
}
bool sBitmapManager::Frame()
{
	return true;
}
bool sBitmapManager::Render()
{
	return true;
}
bool sBitmapManager::Release()
{
	for (m_iter = m_List.begin();
		m_iter != m_List.end();
		m_iter++)
	{
		delete (*m_iter).second;
	}
	m_List.clear();
	return true;
}

sBitmapManager::sBitmapManager()
{
	szDefaultPath = L"../../data/bitmap/";
}

sBitmapManager::~sBitmapManager()
{

}