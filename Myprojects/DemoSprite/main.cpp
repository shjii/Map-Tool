#include "main.h"

bool	main::GameDataLoad(const TCHAR* pszLoad)
{
	TCHAR pBuffer[256] = { 0, };
	TCHAR pTemp[256] = { 0, };

	int iNumSprite = 0;
	FILE* fp_src;
	_wfopen_s(&fp_src, pszLoad, _T("rt"));
	if (fp_src == NULL) return false;

	_fgetts(pBuffer, _countof(pBuffer), fp_src);
	_stscanf_s(pBuffer, _T("%s%d%s"), pTemp, _countof(pTemp), &iNumSprite);
	m_rtSpriteList.resize(iNumSprite);

	for (int iCnt = 0; iCnt < iNumSprite; iCnt++)
	{
		int iNumFrame = 0;
		_fgetts(pBuffer, _countof(pBuffer), fp_src);
		_stscanf_s(pBuffer, _T("%s%d"), pTemp, _countof(pTemp), &iNumFrame);

		RECT rt;
		for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
		{
			_fgetts(pBuffer, _countof(pBuffer), fp_src);
			_stscanf_s(pBuffer, _T("%s %d %d %d %d"), pTemp, _countof(pTemp), 
						&rt.left, &rt.top, &rt.right, &rt.bottom);
			m_rtSpriteList[iCnt].push_back(rt);
		}
	}
	fclose(fp_src);
	return true;
}

bool	main::Init()
{
	GameDataLoad(L"rtSprite.txt");

	for (int iEffect = 0; iEffect < m_rtSpriteList.size(); iEffect++)
	{
		sEffect* pEffect = new sEffect;
		pEffect->Init();
		pEffect->Load(L"../../data/bitmap/bitmap1.bmp",
			L"../../data/bitmap/bitmap1Mask.bmp");
		RECT rtSrc = m_rtSpriteList[iEffect][0];
		sPoint p = { 0,0 };
		pEffect->Set(p,rtSrc);
		pEffect->SetSprite(m_rtSpriteList[iEffect]);
		m_EffectList.push_back(pEffect);
	}
	return true;
}
bool	main::Frame()
{
	if (g_KeyMap.bRightClick >= 1)
	{
		sEffectInfo info;
		info.bDead = false;
		info.id = rand() % 3;
		info.iMaxCount = m_EffectList[info.id]->m_rtList.size();
		info.m_fStep = 1.0f / m_EffectList[info.id]->m_rtList.size();
		info.p = {g_InPut.GetPos().x , g_InPut.GetPos().y };
		m_List.push_back(info);
	}

	for (vector<sEffectInfo>::iterator iter = m_List.begin(); iter != m_List.end();)
	{
		iter->Frame();
		if (iter->bDead == true)
		{
			iter = m_List.erase(iter);
			continue;
		}
		else
		{
			iter++;
		}
	}
	return true;
}
bool	main::Render()
{
	for (auto& pInfo : m_List)
	{
		sPoint p = {pInfo.p.x, pInfo.p.y};
		m_EffectList[pInfo.id]->Set(p, m_EffectList[pInfo.id]->m_rtList[pInfo.m_iRectIndex]);
		m_EffectList[pInfo.id]->Render();
	}
	return true;
}
bool	main::Release()
{
	for (auto pEffect : m_EffectList)
	{
		pEffect->Release();
	}
	return true;
}

GAMERUN;