#pragma once
#include "sBitmapManager.h"
#include "sCollision.h"
enum sObjectType {
	BACKGROUND = 0,
	UICONTROL,
	USER ,
	NPC  
};

enum sControlState {
	DEFAULT = 0,
	PUSH,
	SELECT,
	DISABLE,
};

struct sObjAttribute
{
	int iObjType;
	wstring m_szName;
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
	bool Middlecheck;
	bool jump;
	bool surface;
	float m_fTimer;
public:
	bool	CollisionCheck;
	int HP;
	int			m_iObjectType;
	wstring		m_szName;
public:
	sBitmap*	m_pMaskBmp;
	sBitmap*	m_pColorBmp;
	std::vector<sBitmap*> m_StateBitmap;
	RECT		m_rtSrc;
	RECT		m_rtDesk;
	bool		m_bColorKey;
	DWORD		m_dwColorKey;
	sPoint		m_direction;
public:
	RECT		m_rtCollide;
	sPoint		m_ptPos;
	float		m_fSpeed;
	sPoint		m_ptDirection;
	bool		m_bDead;
	float		m_fLifeTime;
public:
	virtual bool  Init();
	virtual bool  Frame();
	virtual bool  Render();
	virtual bool  Release();
	virtual void  SetPos(sPoint p);
	virtual void  Set(sPoint p, RECT rtSrc, float fSpeed = 200.0f);
	virtual void GetGun() {};
	virtual void  SetPosition(sPoint p);
	virtual void  Set(RECT rtSrc, RECT rtDesk, float fSpeed = 200.0f);
	virtual bool  Load(const TCHAR* color,
		const TCHAR* mask = nullptr,
		DWORD dwColor = RGB(255,0,255));
	virtual void  DrawColorKey();
	virtual bool GravitationalAction();
	virtual void Attack(sObject* pPlayer) {};
	virtual void BAttack(sObject* pPlayer) {};
	virtual void AddEffect(wstring name, sPoint pos) {};
	virtual bool    death() { return true; };
public:
	virtual void SetTransition(DWORD dwEevnt) {};
	virtual void Damage(int a) {};

public:
	sObject();
	virtual ~sObject();
};

