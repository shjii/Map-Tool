#pragma once
#include "sBitmapManager.h"
#include "sCollision.h"

enum sObjectType {
	BACKGROUND = 0,
	UICONTROL,
	USER ,
	NPC  
};
struct sObjAttribute
{
	int iObjType;
	wstring colorbitmap;
	wstring pushbitmap;
	wstring selectbitmap;
	wstring disbitmap;
	wstring maskbitmap;
	sPoint pos;
	RECT rtSrc;
	RECT rtDesk;
	bool bColorKey;
	DWORD dwColor;
};


class sObject
{
public:
	sBitmap* m_pMaskBmp;
	sBitmap* m_pColorBmp;
	vector<sBitmap*> m_StateBitmap;
	RECT m_rtSrc;
	RECT m_rtDesk;
	float m_fSpeed;
	sPoint m_ptPos;
	bool m_bColorKey;
	DWORD m_dwColorKey;

public:
	virtual bool	Init()		;
	virtual bool	Frame()		;
	virtual bool	Render()	;
	virtual bool	Release()	;
	virtual void	SetPosition(sPoint p);
	virtual void	Set(RECT rtSrc, RECT rtDesk, float fSpeed = 200.0f);
	virtual bool	Load(const TCHAR* color, const TCHAR* mask = nullptr);
	virtual bool	Load(const TCHAR* color, DWORD dwColor);
	virtual void	DrawColorKey();
public:
	sObject();
	virtual ~sObject();
};

