#include "sScene.h"
#include "sObjectManager.h"
sGameUser*	 sScene::m_pGamePlayer = nullptr;
sScene*	sScene::m_pCurrentScene = nullptr;

int sScene::NextScene()
{ return iNextScene; }

bool sScene::ReSet()
{
	return true;
}

void sScene::AddEffect(wstring name, sPoint pos)
{
	sEffect* pEffect = (sEffect*)g_objectMgr.GetPtr(name);
	if (pEffect == nullptr) return;
	sEffectInfo info;
	info.m_csNmae = name;
	info.m_bDead = false;
	info.m_fLifeTime = 1.0f;
	info.iMaxCount = pEffect->m_rtList.size();
	info.m_fStep = 1.0f / pEffect->m_rtList.size();
	info.m_fStep = 0.1f;
	info.p = { pos.x , pos.y};
	m_EffectList.push_back(info);
}

void    sScene::Create(wstring name, sPoint pos)
{
	for (sObjAttribute& info : m_ObjAttribute)
	{
		if (info.m_szName == name)
		{
			sObject* pObj = NewObj(info.iObjType);
			pObj->m_szName = info.m_szName;
			pObj->Init();
			if (pObj->m_iObjectType == 100)
			{
				if (pObj->m_szName == L"Tank")
				{
					pObj->HP = 20;
				}
			}
			if (info.bColorKey == false)
			{
				pObj->Load(info.colorbitmap.c_str(),
					info.maskbitmap.c_str());
			}
			else
			{
				pObj->Load(info.colorbitmap.c_str(),
					nullptr,
					info.dwColor);
			}

			pObj->m_StateBitmap.resize(4);
			pObj->m_StateBitmap[DEFAULT] = pObj->m_pColorBmp;

			if (!info.pushbitmap.empty());
			{
				pObj->m_StateBitmap[PUSH] = (g_BitMapMar.Load(info.pushbitmap.c_str()));
			}
			if (!info.selectbitmap.empty());
			{
				pObj->m_StateBitmap[SELECT] = (g_BitMapMar.Load(info.selectbitmap.c_str()));
			}
			if (!info.disbitmap.empty());
			{
				pObj->m_StateBitmap[DISABLE] = (g_BitMapMar.Load(info.disbitmap.c_str()));
			}
			pObj->Set(info.rtSrc, info.rtDesk);
			pObj->SetPosition(pos);
			if (info.iObjType == 300)
			{
				m_BackobjList.push_back(pObj);
			}
			if (info.iObjType == 100)
			{
				m_pGamePlayer = (sGameUser*)pObj;
			}
			if (info.iObjType == 200)
			{
				m_objList.push_back(pObj);
			}
			if (info.iObjType == 301)
			{
				m_UIobjList.push_back(pObj);
			}
			if (info.iObjType == 302)
			{
				m_FloorList.push_back(pObj);
			}
			if (info.iObjType >= 303)
			{
				m_EventList.push_back(pObj);
			}
		}
	}
}


bool	sScene::GameDataLoad(const TCHAR* pszLoad, vector<sSpriteInfo>& List)
{
	TCHAR pBuffer[256] = { 0, };
	TCHAR pTemp[256] = { 0, };

	int iNumSprite = 0;
	FILE* fp_src;
	_wfopen_s(&fp_src, pszLoad, _T("rt"));
	if (fp_src == NULL) return false;

	_fgetts(pBuffer, _countof(pBuffer), fp_src);
	_stscanf_s(pBuffer, _T("%s%d%s"), pTemp, _countof(pTemp), &iNumSprite);
	List.resize(iNumSprite);

	for (int iCnt = 0; iCnt < iNumSprite; iCnt++)
	{
		int iNumFrame = 0;
		_fgetts(pBuffer, _countof(pBuffer), fp_src);
		_stscanf_s(pBuffer, _T("%s %d"), pTemp, _countof(pTemp), &iNumFrame);
		List[iCnt].szName = pTemp;
		RECT rt;
		for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
		{
			_fgetts(pBuffer, _countof(pBuffer), fp_src);
			_stscanf_s(pBuffer, _T("%s %d %d %d %d"), pTemp, _countof(pTemp),
				&rt.left, &rt.top, &rt.right, &rt.bottom);
			List[iCnt].rtArray.push_back(rt);
		}
	}
	fclose(fp_src);
	return true;
}


sObject* sScene::NewObj(int iType)
{
	sObject* pObject = nullptr;
	switch (iType)
	{
	case 0: {pObject = new sObject; } break;
	case 100: {pObject = new sGameUser; } break;
	case 200: {pObject = new sNpcObject;
		sNpcObject* pNpc = dynamic_cast<sNpcObject*>(pObject);
		if (pNpc != nullptr)
		{
			pNpc->FSM();
		} } break;
	case 300: {pObject = new sUserInterface; } break;
	case 301: {pObject = new sButton; } break;
	case 302: {pObject = new sFloor; }break;
	case 303: {pObject = new sEcentScene; }break;
	case 304: {pObject = new SItem; }break;
	}
	return pObject;
}
bool sScene::LoadScript(const TCHAR* filename)
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
			_stscanf_s(szBuffer, _T("%d %s"), &objinfo.iObjType, szTemp, _countof(szTemp));
			wstring a = szTemp;
			objinfo.m_szName = a;

			GetBitmapLoad(fp, objinfo.colorbitmap);
			GetBitmapLoad(fp, objinfo.pushbitmap);
			GetBitmapLoad(fp, objinfo.selectbitmap);
			GetBitmapLoad(fp, objinfo.disbitmap);
			GetBitmapLoad(fp, objinfo.maskbitmap);

			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%f%f"),
				&objinfo.pos.x,
				&objinfo.pos.y);
			objinfo.rtDesk.left = objinfo.pos.x;
			objinfo.rtDesk.top = objinfo.pos.y;
			//	-1, -1, -1, -1,
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%d%d%d%d"),
				&objinfo.rtSrc.left,
				&objinfo.rtSrc.top,
				&objinfo.rtSrc.right,
				&objinfo.rtSrc.bottom);
			//	-1, -1,
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%d%d"),
				&objinfo.rtDesk.right,
				&objinfo.rtDesk.bottom);
			//	0,255, 0, 255
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
void	sScene::GetBitmapLoad(FILE* fp, wstring& ret)
{
	TCHAR szBuffer[256] = { 0, };
	TCHAR szTemp[256] = { 0, };
	//	nullptr,
	_fgetts(szBuffer, _countof(szBuffer), fp);
	_stscanf_s(szBuffer, _T("%s"), szTemp, _countof(szTemp));
	wstring maskbmp = szTemp;
	if (maskbmp != L"nullptr")
	{
		ret = maskbmp;
	}
}
bool	sScene::Load(const TCHAR* filename)
{
	Init();
	if (!LoadScript(filename))
	{
		return false;
	}
	for (sObjAttribute& info : m_ObjAttribute)
	{
		sObject* pObj = NewObj(info.iObjType);
		pObj->m_szName = info.m_szName;
		pObj->Init();
		if (pObj->m_iObjectType == 100)
		{
			if (pObj->m_szName == L"Tank")
			{
				pObj->HP = 20;
			}
		}
		if (info.bColorKey == false)
		{
			pObj->Load(info.colorbitmap.c_str(),
				info.maskbitmap.c_str());
		}
		else
		{
			pObj->Load(info.colorbitmap.c_str(),
				nullptr,
				info.dwColor);
		}

		pObj->m_StateBitmap.resize(4);
		pObj->m_StateBitmap[DEFAULT] = pObj->m_pColorBmp;

		if (!info.pushbitmap.empty());
		{
			pObj->m_StateBitmap[PUSH] = (g_BitMapMar.Load(info.pushbitmap.c_str()));
		}
		if (!info.selectbitmap.empty());
		{
			pObj->m_StateBitmap[SELECT] = (g_BitMapMar.Load(info.selectbitmap.c_str()));
		}
		if (!info.disbitmap.empty());
		{
			pObj->m_StateBitmap[DISABLE] = (g_BitMapMar.Load(info.disbitmap.c_str()));
		}

		pObj->SetPosition(info.pos);
		pObj->Set(info.rtSrc, info.rtDesk);
		if (info.iObjType == 300)
		{
			m_BackobjList.push_back(pObj);
		}
		if (info.iObjType == 100)
		{
			m_pGamePlayer = (sGameUser*)pObj;
		}
		if (info.iObjType == 200)
		{
			m_objList.push_back(pObj);
		}
		if (info.iObjType == 301)
		{
			m_UIobjList.push_back(pObj);
		}
		if (info.iObjType == 302)
		{
			m_FloorList.push_back(pObj);
		}
		if (info.iObjType >= 303)
		{
			m_EventList.push_back(pObj);
		}
	}
	GameDataLoad(L"rtSprite.txt", m_rtSpriteList);
	for (int iEffect = 0; iEffect < m_rtSpriteList.size(); iEffect++)
	{
		sEffect* pEffect = new sEffect;
		pEffect->Init();
		pEffect->m_szName = m_rtSpriteList[iEffect].szName;
		pEffect->Load(L"../../data/bitmap/SFX.bmp");
		RECT rtSrc = m_rtSpriteList[iEffect].rtArray[0];
		sPoint p = { 0,0 };
		pEffect->Set(p, rtSrc);
		pEffect->SetSprite(m_rtSpriteList[iEffect].rtArray);
		g_objectMgr.Add(pEffect);
	}

	GameDataLoad(L"animation.txt", m_AnimationList);
	for (int iEffect = 0; iEffect < m_AnimationList.size(); iEffect++)
	{
		sEffect* pEffect = new sEffect;
		pEffect->Init();
		pEffect->m_szName = m_AnimationList[iEffect].szName;
		pEffect->Load(L"../../data/bitmap/player.bmp");
		RECT rtSrc = m_AnimationList[iEffect].rtArray[0];
		sPoint p = { 0,0 };
		pEffect->Set(p, rtSrc);
		pEffect->SetSprite(m_AnimationList[iEffect].rtArray);
		g_objectMgr.Add(pEffect);
	}
	GameDataLoad(L"humananimation.txt", humanList);
	for (int iEffect = 0; iEffect < humanList.size(); iEffect++)
	{
		sEffect* pEffect = new sEffect;
		pEffect->Init();
		pEffect->m_szName = humanList[iEffect].szName;
		pEffect->Load(L"../../data/bitmap/hm.bmp");
		RECT rtSrc = humanList[iEffect].rtArray[0];
		sPoint p = { 0,0 };
		pEffect->Set(p, rtSrc);
		pEffect->SetSprite(humanList[iEffect].rtArray);
		g_objectMgr.Add(pEffect);
	}
	GameDataLoad(L"Tankanimation.txt", TankList);
	for (int iEffect = 0; iEffect < TankList.size(); iEffect++)
	{
		sEffect* pEffect = new sEffect;
		pEffect->Init();
		pEffect->m_szName = TankList[iEffect].szName;
		pEffect->Load(L"../../data/bitmap/mt.bmp");
		RECT rtSrc = TankList[iEffect].rtArray[0];
		sPoint p = { 0,0 };
		pEffect->Set(p, rtSrc);
		pEffect->SetSprite(TankList[iEffect].rtArray);
		g_objectMgr.Add(pEffect);
	}
	GameDataLoad(L"Boosanimation.txt", BossList);
	for (int iEffect = 0; iEffect < BossList.size(); iEffect++)
	{
		sEffect* pEffect = new sEffect;
		pEffect->Init();
		pEffect->m_szName = BossList[iEffect].szName;
		pEffect->Load(L"../../data/bitmap/BOSS.bmp");
		RECT rtSrc = BossList[iEffect].rtArray[0];
		sPoint p = { 0,0 };
		pEffect->Set(p, rtSrc);
		pEffect->SetSprite(BossList[iEffect].rtArray);
		g_objectMgr.Add(pEffect);
	}
	GameDataLoad(L"NPC.txt", NPCList);
	for (int iEffect = 0; iEffect < NPCList.size(); iEffect++)
	{
		sEffect* pEffect = new sEffect;
		pEffect->Init();
		pEffect->m_szName = NPCList[iEffect].szName;
		pEffect->Load(L"../../data/bitmap/NPC.bmp");
		RECT rtSrc = NPCList[iEffect].rtArray[0];
		sPoint p = { 0,0 };
		pEffect->Set(p, rtSrc);
		pEffect->SetSprite(NPCList[iEffect].rtArray);
		g_objectMgr.Add(pEffect);
	}
	GameDataLoad(L"Item.txt", ItemList);
	for (int iEffect = 0; iEffect < ItemList.size(); iEffect++)
	{
		sEffect* pEffect = new sEffect;
		pEffect->Init();
		pEffect->m_szName = ItemList[iEffect].szName;
		pEffect->Load(L"../../data/bitmap/Items.bmp");
		RECT rtSrc = ItemList[iEffect].rtArray[0];
		sPoint p = { 0,0 };
		pEffect->Set(p, rtSrc);
		pEffect->SetSprite(ItemList[iEffect].rtArray);
		g_objectMgr.Add(pEffect);
	}
	GameDataLoad(L"Tank.txt", TankList);
	for (int iEffect = 0; iEffect < TankList.size(); iEffect++)
	{
		sEffect* pEffect = new sEffect;
		pEffect->Init();
		pEffect->m_szName = TankList[iEffect].szName;
		pEffect->Load(L"../../data/bitmap/Tank.bmp");
		RECT rtSrc = TankList[iEffect].rtArray[0];
		sPoint p = { 0,0 };
		pEffect->Set(p, rtSrc);
		pEffect->SetSprite(TankList[iEffect].rtArray);
		g_objectMgr.Add(pEffect);
	}
	return true;
}

bool	sScene::Init()
{
	return true;
}
bool	sScene::Frame()
{
	for (sObject* pObj : m_BackobjList)
	{
		pObj->Frame();
	}
	for (sObject* pObj : m_UIobjList)
	{
		pObj->Frame();
	}
	for (sObject* pObj : m_objList)
	{
		if (pObj->CollisionCheck == true && pObj->m_szName == L"NPC")
		{
			Create(L"GUN", pObj->m_ptPos);
		}
		if (pObj->CollisionCheck == true && pObj->m_szName == L"Boos")
		{
			pObj->CollisionCheck = false;
			return false;
			// 게임 엔딩
		}
		pObj->surface = false;
		for (sObject* pFloorObj : m_FloorList)
		{
			if (sCollision::Rect2Rect(pFloorObj->m_rtCollide, pObj->m_rtCollide))
			{
				if (pObj->m_ptPos.y - (pObj->m_rtSrc.bottom / 2) <= pFloorObj->m_ptPos.y)
				{
					pObj->surface = true;
				}
			}
		}
		pObj->Frame();
	}
	for (sObject* pObj : m_EventList)
	{
		pObj->Frame();
		for (sObject* pFloorObj : m_FloorList)
		{
			if (sCollision::Rect2Rect(pFloorObj->m_rtCollide, pObj->m_rtCollide))
			{
				if (pObj->m_ptPos.y - (pObj->m_rtSrc.bottom / 2) <= pFloorObj->m_ptPos.y)
				{
					pObj->surface = true;
				}
			}
		}
	}
	//
	for (sObject* pObj : m_FloorList)
	{
		pObj->Frame();
	}
	//
	for (vector<sEffectInfo>::iterator iter = m_EffectList.begin(); iter != m_EffectList.end();)
	{
		iter->Frame();
		if (iter->m_bDead == true)
		{
			iter = m_EffectList.erase(iter);
			continue;
		}
		else
		{
			iter++;
		}
	}
	for (vector<sObject*>::iterator iter = m_EventList.begin(); iter != m_EventList.end();)
	{
		if ((*iter)->m_bDead == true)
		{
			iter = m_EventList.erase(iter);
			continue;
		}
		else
		{
			iter++;
		}
	}
	return true;
}
bool	sScene::Render()
{

	for (sObject* pObj : m_BackobjList)
	{
		pObj->Render();
	}
	for (sObject* pObj : m_UIobjList)
	{
		pObj->Render();
	}
	for (sObject* pObj : m_objList)
	{
		if (pObj->m_bDead == false)
		{
			pObj->Render();
		}
	}

	// 이것만 바꾸면댕
	for (sObject* pObj : m_EventList)
	{
		pObj->Render();
	}

	for (sObject* pObj : m_FloorList)
	{
		pObj->Render();
	}
	//


	for (auto& pInfo : m_EffectList)
	{
		sEffect* pEffect = (sEffect*)g_objectMgr.GetPtr(pInfo.m_csNmae);
		pEffect->Set(pInfo.p, pEffect->m_rtList[pInfo.m_iRectIndex]);
		pEffect->Render();
	}

	return true;
}
bool	sScene::Release()
{
	for (sObject* pObj : m_BackobjList)
	{
		pObj->Release();
		delete pObj;
	}
	for (sObject* pObj : m_UIobjList)
	{
		pObj->Release();
		delete pObj;
	}
	for (sObject* pObj : m_objList)
	{
		pObj->Release();
		delete pObj;
	}
	//
	for (sObject* pObj : m_FloorList)
	{
		pObj->Release();
		delete pObj;
	}
	for (sObject* pObj : m_EventList)
	{
		pObj->Release();
		delete pObj;
	}
	//
	delete m_pGamePlayer;
	m_pGamePlayer = nullptr;
	m_BackobjList.clear();
	m_EventList.clear();
	m_UIobjList.clear();
	m_objList.clear();
	m_FloorList.clear();
	//m_objList.clear();
	//m_ObjAttribute.clear();
	return true;
}