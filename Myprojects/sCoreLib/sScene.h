#pragma once
#include "sUserInterface.h"
#include "sNpcObject.h"
#include "sGameUser.h"
#include "sEffect.h"
#include "Floor.h"
#include "sEcentScene.h"
#include "SItem.h"

struct sSpriteInfo
{
	RECT_APRAY rtArray;
	wstring szName;
};

class sScene
{
public:
	int iNextScene;
	static sScene*				m_pCurrentScene;
	vector<sObjAttribute> m_ObjAttribute;
	vector<sObject*> m_objList;
	vector<sObject*> m_UIobjList;
	vector<sObject*> m_ItemobjList;
	vector<sObject*> m_BackobjList;
	vector<sObject*> m_FloorList;
	vector<sObject*> m_EventList;
	vector<sObject*> m_ItemList;
	static sGameUser*	 m_pGamePlayer;
	vector<sSpriteInfo> m_rtSpriteList;
	vector<sSpriteInfo> m_AnimationList;
	vector<sSpriteInfo> humanList;
	vector<sSpriteInfo> TankList;
	vector<sSpriteInfo> BossList;
	vector<sSpriteInfo> NPCList;
	vector<sSpriteInfo> ItemList;
	vector<sEffectInfo> m_EffectList;
	int m_iSceneID;
	bool m_bSceneChange;
public:
	virtual int NextScene();
	virtual bool	Init()		;
	virtual bool	Frame()		;
	virtual bool	Render()	;
	virtual bool	Release()	;
	virtual bool	ReSet()		;
	
	void	GetBitmapLoad(FILE* fp, wstring& ret);
	bool	Load(const TCHAR* filename);
	void    Create(wstring name, sPoint pos);
public:
	void AddEffect(wstring name, sPoint pos);
	bool	GameDataLoad(const TCHAR* pszLoad, vector<sSpriteInfo>& List);
	sObject* NewObj(int iType);
	bool LoadScript(const TCHAR* filename);
	sScene()
	{
		m_iSceneID = 0;
		m_bSceneChange = false;
	}
};

