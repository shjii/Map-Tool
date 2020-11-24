#include "main.h"

bool	main::Init()
{
	if (!LoadScript(L"ui.txt"))
	{
		return false;
	}
	if (!LoadScript(L"objectinfo.txt"))
	{
		return false;
	}
	for (sObjAttribute& info : m_ObjAttribute)
	{
		sObject* pObj = NewObj(info.iObjType);
		pObj->Init();
		if (info.bColorKey == false)
		{
			pObj->Load(info.colorbitmap.c_str(), info.maskbitmap.c_str());
		}
		else
		{
			pObj->Load(info.colorbitmap.c_str(), info.dwColor);
		}
		pObj->SetPosition(info.pos);
		pObj->Set(info.rtSrc, info.rtDesk);
		m_objList.push_back(pObj);
	}
	return true;
}
bool	main::Frame()
{
	for (sObject* pObj : m_objList)
	{
		pObj->Frame();
	}
	return true;
}
bool	main::Render()
{
	for (sObject* pObj : m_objList)
	{
		pObj->Render();
	}
	return true;
}
bool	main::Release()
{
	for (sObject* pObj : m_objList)
	{
		pObj->Release();
		delete pObj;
	}
	m_objList.clear();
	m_ObjAttribute.clear();
	return true;
}
sObject* main::NewObj(int iType)
{
	sObject* pObject = nullptr;
	switch (iType)
	{
	case 0: {
		pObject = new sUserInterface; }break;
	case 100: {
		pObject = new sGameUser; }break;
	case 200: {
		pObject = new sNpcObject; }break;
	case 300: {
		pObject = new sUserInterface; }break;
	}
	return pObject;
}
bool main::LoadScript(const TCHAR* filename)
{
	FILE* fp = nullptr;
	_wfopen_s(&fp, filename, L"rt");
	TCHAR szBuffer[256] = { 0, };
	TCHAR szTemp[256] = { 0, };

	if (fp != nullptr)
	{
		sObjAttribute objinfo;
		int iMaxCount = 0;
		_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
		_stscanf_s(szBuffer, _T("%d"), &iMaxCount);
		for (int iObj = 0; iObj < iMaxCount; iObj++)
		{
			_fgetts(szBuffer, _countof(szBuffer), fp);

			_fgetts(szBuffer, _countof(szBuffer), fp);
			_stscanf_s(szBuffer, _T("%d"), &objinfo.iObjType);

			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%s"), szTemp, _countof(szTemp));
			objinfo.colorbitmap = szTemp;

			_fgetts(szBuffer, _countof(szBuffer), fp);
			_stscanf_s(szBuffer, _T("%s"), szTemp, _countof(szTemp));
			wstring maskbmp = szTemp;
			if (maskbmp != L"nullptr")
			{
				objinfo.maskbitmap = maskbmp;
			}
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%f%f"),
				&objinfo.pos.x,
				&objinfo.pos.y);
			objinfo.rtDesk.left = objinfo.pos.x;
			objinfo.rtDesk.top = objinfo.pos.y;

			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%d%d%d%d"),
				&objinfo.rtSrc.left,
				&objinfo.rtSrc.top,
				&objinfo.rtSrc.right,
				&objinfo.rtSrc.bottom);

			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%d%d"),
				&objinfo.rtDesk.right,
				&objinfo.rtDesk.bottom);

			int mask = 0;
			int r = 0;
			int g = 0;
			int b = 0;
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%d%d%d%d"),
				&mask,
				&r, &g, &b);
			if (mask == 0)
				objinfo.bColorKey = false;
			else
				objinfo.bColorKey = true;
			objinfo.dwColor = RGB(r, g, b);
			m_ObjAttribute.push_back(objinfo);
		}
	}
	fclose(fp);

	return true;
}
GAMERUN;