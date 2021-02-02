#pragma once
#include "sEffect.h"
class sObjectManager : public  GetSing<sObjectManager>
{
public:
	friend class GetSing<sObjectManager>;
private:
	map<wstring, sObject*> m_List;
	map<wstring, sObject*>::iterator m_iter;
	wstring m_szDfaultPath;
public:
	sObject* Add(sObject* pAddObject);
	sObject* Load(const TCHAR* filename, DWORD dwMaskColor = RGB(255,0,255));
	sObject* GetPtr(wstring filename);
	bool	Init()		;
	bool	Frame()		;
	bool	Render()	;
	bool	Release()	;
private:
	sObjectManager();
public:
	~sObjectManager();
};
#define g_objectMgr sObjectManager::GetInstance()

