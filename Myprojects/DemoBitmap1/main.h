#pragma once
#include "sCore.h"
#include "sUserInterface.h"
#include "sGameUser.h"
#include "sNpcObject.h"

enum sObjectType {
	BACKGROUND = 0,
	UICONTROL = 100,
	USER = 200,
	NPC = 300
};
struct sObjAttribute
{
	int iObjType;
	wstring colorbitmap;
	wstring maskbitmap;
	sPoint pos;
	RECT rtSrc;
	RECT rtDesk;
	bool bColorKey;
	DWORD dwColor;
};

class main : public sCore
{
public:
	vector<sObjAttribute> m_ObjAttribute;
	list<sObject*> m_objList;

	sPoint m_ptInitHeroPos;
	float m_fSpeed;
public:
	bool	Init()		override;
	bool	Frame()		override;
	bool	Render()	override;
	bool	Release()	override;
public:
	sObject* NewObj(int iType);
	bool LoadScript(const TCHAR* filename);
};

