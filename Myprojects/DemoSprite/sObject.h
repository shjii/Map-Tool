#pragma once
#include "sBitmapManager.h"
class sObject
{
public:
	sBitmap* m_pMaskBmp;
	sBitmap* m_pColorBmp;
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

	virtual void	SetPos(sPoint p);
	virtual void	Set(sPoint p, RECT rtSrc, float fSpeed = 200.0f);

	virtual void	SetPosition(sPoint p);
	virtual void	Set(RECT rtSrc, RECT rtDesk, float fSpeed = 200.0f);

	virtual bool	Load(const TCHAR* color, const TCHAR* mask = nullptr);
	virtual bool	Load(const TCHAR* color, DWORD dwColor);
	virtual void	DrawColorKey();
public:
	sObject();
	virtual ~sObject();
};

