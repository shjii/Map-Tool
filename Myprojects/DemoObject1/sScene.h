#pragma once
#include "sUserInterface.h"
#include "sNpcObject.h"
#include "sGameUser.h"

class sScene
{
public:
	vector<sObjAttribute> m_ObjAttribute;
	vector<sObject*> m_objList;
	int m_iSceneID;
	bool m_bSceneChange;
public:
	virtual bool	Init()		;
	virtual bool	Frame()		;
	virtual bool	Render()	;
	virtual bool	Release()	;
	void	GetBitmapLoad(FILE* fp, wstring& ret);
	bool	Load(const TCHAR* filename);
public:
	sObject* NewObj(int iType);
	bool LoadScript(const TCHAR* filename);
	sScene()
	{
		m_iSceneID = 0;
		m_bSceneChange = false;
	}
};

