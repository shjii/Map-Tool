#include "sAnimation.h"
#include "sObjectManager.h"
#include "sScene.h"

void sAnimation::SetOwner(sObject* _Owner)
{
	m_Owner = _Owner;
}

bool sAnimation::SetAnimationUP(wstring _name)
{
	if (_name != m_pUpperBody->m_szName)
	{
		m_pUpperBody = (sEffect*)g_objectMgr.GetPtr(_name);
		return true;
	}
	return false;
}

bool sAnimation::SetAnimationLO(wstring _name)
{
	if (_name != m_pLowerBody->m_szName)
	{
		m_pLowerBody = (sEffect*)g_objectMgr.GetPtr(_name);
		return true;
	}
	return false;
}

bool sAnimation::SetAnimation(wstring _name)
{
	Body = (sEffect*)g_objectMgr.GetPtr(_name);
	return true;
}

bool sAnimation::Frame()
	{
	if (Body == nullptr)
	{
		m_pLowerBody->m_fTimer += g_fSecondPerFrame;
		if (m_pLowerBody->m_fTimer >= m_pLowerBody->m_fStep)
		{
			m_pLowerBody->m_iRectIndex++;
			if (m_pLowerBody->m_rtList.size() <= m_pLowerBody->m_iRectIndex)
			{
				m_pLowerBody->m_iRectIndex = 0;
			}
			m_pLowerBody->m_fTimer -= m_pLowerBody->m_fStep;
		}
		m_pLowerBody->Set(m_pLowerBody->m_rtList[m_pLowerBody->m_iRectIndex], { (LONG)m_pLowerBody->m_ptPos.x ,
			(LONG)m_pLowerBody->m_ptPos.y,
			m_pLowerBody->m_rtList[m_pLowerBody->m_iRectIndex].right,
			m_pLowerBody->m_rtList[m_pLowerBody->m_iRectIndex].bottom });
		//Set(sPoint p, RECT rtSrc, float fSpeed)
		///////////////////////////////
		m_pUpperBody->m_fTimer += g_fSecondPerFrame;
		if (m_pUpperBody->m_fTimer >= m_pUpperBody->m_fStep)
		{
			m_pUpperBody->m_iRectIndex++;
			if (m_pUpperBody->m_rtList.size() <= m_pUpperBody->m_iRectIndex)
			{
				m_pUpperBody->m_iRectIndex = 0;
			}
			m_pUpperBody->m_fTimer -= m_pUpperBody->m_fStep;
		}
		m_pUpperBody->Set(m_pUpperBody->m_rtList[m_pUpperBody->m_iRectIndex], { (LONG)m_pUpperBody->m_ptPos.x ,
			(LONG)m_pUpperBody->m_ptPos.y,
			m_pUpperBody->m_rtList[m_pUpperBody->m_iRectIndex].right,
			m_pUpperBody->m_rtList[m_pUpperBody->m_iRectIndex].bottom });
		//¼öÁ¤ sScene::m_pGamePlayer
		m_pUpperBody->m_ptPos = m_Owner->m_ptPos;
		m_pUpperBody->m_rtCollide = m_Owner->m_rtCollide;
		m_pLowerBody->m_ptPos = m_Owner->m_ptPos;
		m_pLowerBody->m_rtCollide = m_Owner->m_rtCollide;
	}
	else
	{
		Body->m_fTimer += g_fSecondPerFrame;
		if (Body->m_fTimer >= Body->m_fStep)
		{
			Body->m_iRectIndex++;
			if (Body->m_rtList.size() <= Body->m_iRectIndex)
			{
				Body->m_iRectIndex = 0;
			}
			Body->m_fTimer -= Body->m_fStep;
		}
		Body->Set(Body->m_rtList[Body->m_iRectIndex], { (LONG)Body->m_ptPos.x ,
			(LONG)Body->m_ptPos.y,
			Body->m_rtList[Body->m_iRectIndex].right,
			Body->m_rtList[Body->m_iRectIndex].bottom });
		Body->m_ptPos = m_Owner->m_ptPos;
		Body->m_rtCollide = m_Owner->m_rtCollide;
	}
	return true;
}

bool sAnimation::Render()
{
	//if (pos > 0)
	{
		m_BlendFunction.BlendOp = AC_SRC_OVER;
		m_BlendFunction.BlendFlags = 0;
		m_BlendFunction.SourceConstantAlpha = 255;
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
		if (Body == nullptr)
		{
			AlphaBlend(g_hOffScreenDC,
				m_Owner->m_rtCollide.left, m_Owner->m_rtCollide.bottom - (m_Owner->m_rtDesk.bottom / 2),
				m_pLowerBody->m_rtDesk.right * 2, m_pLowerBody->m_rtDesk.bottom * 2,
				m_pLowerBody->m_pColorBmp->m_hMemDC,
				m_pLowerBody->m_rtSrc.left, m_pLowerBody->m_rtSrc.top,
				m_pLowerBody->m_rtSrc.right, m_pLowerBody->m_rtSrc.bottom,
				m_BlendFunction);

			AlphaBlend(g_hOffScreenDC,
				m_Owner->m_rtCollide.left, m_Owner->m_rtCollide.top + (m_pLowerBody->m_rtDesk.bottom / 2) + 8,
				m_pUpperBody->m_rtDesk.right * 2, m_pUpperBody->m_rtDesk.bottom * 2,
				m_pUpperBody->m_pColorBmp->m_hMemDC,
				m_pUpperBody->m_rtSrc.left, m_pUpperBody->m_rtSrc.top,
				m_pUpperBody->m_rtSrc.right, m_pUpperBody->m_rtSrc.bottom,
				m_BlendFunction);
		}
		else
		{
			AlphaBlend(g_hOffScreenDC,
				m_Owner->m_rtCollide.left, m_Owner->m_rtCollide.top,
				Body->m_rtDesk.right * 2, Body->m_rtDesk.bottom * 2,
				Body->m_pColorBmp->m_hMemDC,
				Body->m_rtSrc.left, Body->m_rtSrc.top,
				Body->m_rtSrc.right, Body->m_rtSrc.bottom,
				m_BlendFunction);
		}
	}
	/*else
	{
		if (Body == nullptr)
		{
			BitBlt(a, 0, 0, g_rtClient.right, g_rtClient.bottom, m_pUpperBody->m_pColorBmp->m_hMemDC, 0, 0, NOTSRCCOPY);
			TransparentBlt(g_hOffScreenDC,
				m_Owner->m_rtCollide.left, m_Owner->m_rtCollide.bottom - (m_Owner->m_rtDesk.bottom / 2),
				m_pLowerBody->m_rtDesk.right * 2, m_pLowerBody->m_rtDesk.bottom * 2,
				a,
				m_pLowerBody->m_rtSrc.left, m_pLowerBody->m_rtSrc.top,
				m_pLowerBody->m_rtSrc.right, m_pLowerBody->m_rtSrc.bottom,
				RGB(255,255,255));
			

			AlphaBlend(g_hOffScreenDC,
				m_Owner->m_rtCollide.left, m_Owner->m_rtCollide.top + (m_pLowerBody->m_rtDesk.bottom / 2) + 8,
				m_pUpperBody->m_rtDesk.right * 2, m_pUpperBody->m_rtDesk.bottom * 2,
				m_pUpperBody->m_pColorBmp->m_hMemDC,
				m_pUpperBody->m_rtSrc.left, m_pUpperBody->m_rtSrc.top,
				m_pUpperBody->m_rtSrc.right, m_pUpperBody->m_rtSrc.bottom,
				m_BlendFunction);
		}
		else
		{
			StretchBlt(a,
				m_Owner->m_rtCollide.left, m_Owner->m_rtCollide.top,
				Body->m_rtDesk.right * 2, Body->m_rtDesk.bottom * 2,
				Body->m_pColorBmp->m_hMemDC,
				Body->m_rtSrc.left, Body->m_rtSrc.top,
				Body->m_rtSrc.right, Body->m_rtSrc.bottom,
				SRCCOPY);
			AlphaBlend(g_hOffScreenDC,
				m_Owner->m_rtCollide.left, m_Owner->m_rtCollide.top,
				Body->m_rtDesk.right * 2, Body->m_rtDesk.bottom * 2,
				a,
				Body->m_rtSrc.left, Body->m_rtSrc.top,
				Body->m_rtSrc.right, Body->m_rtSrc.bottom,
				m_BlendFunction);
		}
	}*/
		return true;
}
bool sAnimation::SetP(float a)
{
	pos = a;
	return 0;
}

sAnimation::sAnimation()
{
	a = CreateCompatibleDC(g_hScreenDC);
	m_pUpperBody = (sEffect*)g_objectMgr.GetPtr(L"NPCW");
	m_pLowerBody = (sEffect*)g_objectMgr.GetPtr(L"NPCW");
	Body = nullptr;
}
sAnimation::~sAnimation()
{
	ReleaseDC(g_hWnd, a);
}