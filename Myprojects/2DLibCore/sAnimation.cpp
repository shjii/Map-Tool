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
		m_pUpperBody->m_iRectIndex = 0;
		m_pUpperBody = (sEffect*)g_objectMgr.GetPtr(_name);
		return true;
	}
	return false;
}

bool sAnimation::SetAnimationLO(wstring _name)
{
	if (_name != m_pLowerBody->m_szName)
	{
		m_pLowerBody->m_iRectIndex = 0;
		m_pLowerBody = (sEffect*)g_objectMgr.GetPtr(_name);
		return true;
	}
	return false;
}

bool sAnimation::SetAnimation(wstring _name)
{
	//if (Body != nullptr)
	//{
	//	if (_name != m_pLowerBody->m_szName)
	//	{
			//Body->m_iRectIndex = 0;
			Body = (sEffect*)g_objectMgr.GetPtr(_name);
			return true;
	//	}
	//}
	
	return false;
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
		//수정 sScene::m_pGamePlayer
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

	// 애니매이션을 깔끔하게 하기 위해서 각 모션마다 보정이 필요함..... 시발 
	y = m_pUpperBody->m_rtList[m_pUpperBody->m_iRectIndex].bottom * 2;
	if (y >= 60)
	{
		y -= 10;
	}

	

	if (pos.x > 0)
	{
		if (Body == nullptr)
		{
			Upx = m_Owner->m_rtCollide.left;
			Lox = m_Owner->m_rtCollide.left;
		}
		else
		{
			x = m_Owner->m_rtCollide.left;
		}
		x = m_Owner->m_rtCollide.left;
	}
	else if (pos.x < 0)
	{
		if (Body == nullptr)
		{
			Upx = m_Owner->m_rtCollide.right - m_pUpperBody->m_rtList[m_pUpperBody->m_iRectIndex].right * 2;
			Lox = m_Owner->m_rtCollide.right - m_pLowerBody->m_rtList[m_pLowerBody->m_iRectIndex].right * 2;
		}
		else
		{
			x = m_Owner->m_rtCollide.right - Body->m_rtList[Body->m_iRectIndex].right;
		}
		
	}
	return true;
}

bool sAnimation::Render()
{

	//m_BlendFunction.BlendOp = AC_SRC_OVER;
	//m_BlendFunction.BlendFlags = 0;
	//m_BlendFunction.SourceConstantAlpha = 255;
	//m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	//if (Body == nullptr)
	//{
	//	AlphaBlend(g_hOffScreenDC,
	//		m_Owner->m_rtCollide.left, m_Owner->m_rtCollide.bottom - (m_Owner->m_rtDesk.bottom / 2),
	//		m_pLowerBody->m_rtDesk.right * 2, m_pLowerBody->m_rtDesk.bottom * 2,
	//		m_pLowerBody->m_pColorBmp->m_hMemDC,
	//		m_pLowerBody->m_rtSrc.left, m_pLowerBody->m_rtSrc.top,
	//		m_pLowerBody->m_rtSrc.right, m_pLowerBody->m_rtSrc.bottom,
	//		m_BlendFunction);

	//	AlphaBlend(g_hOffScreenDC,
	//		m_Owner->m_rtCollide.left, m_Owner->m_rtCollide.top + (m_pLowerBody->m_rtDesk.bottom / 2),
	//		m_pUpperBody->m_rtDesk.right * 2, m_pUpperBody->m_rtDesk.bottom * 2,
	//		m_pUpperBody->m_pColorBmp->m_hMemDC,
	//		m_pUpperBody->m_rtSrc.left, m_pUpperBody->m_rtSrc.top,
	//		m_pUpperBody->m_rtSrc.right, m_pUpperBody->m_rtSrc.bottom,
	//		m_BlendFunction);
	//}
	//else
	//{
	//	AlphaBlend(g_hOffScreenDC,
	//		m_Owner->m_rtCollide.left, m_Owner->m_rtCollide.top,
	//		Body->m_rtDesk.right * 2, Body->m_rtDesk.bottom * 2,
	//		Body->m_pColorBmp->m_hMemDC,
	//		Body->m_rtSrc.left, Body->m_rtSrc.top,
	//		Body->m_rtSrc.right, Body->m_rtSrc.bottom,
	//		m_BlendFunction);
	//}

	
	{
		m_BlendFunction.BlendOp = AC_SRC_OVER;
		m_BlendFunction.BlendFlags = 0;
		m_BlendFunction.SourceConstantAlpha = 255;
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
		if (Body == nullptr)
		{
			AlphaBlend(g_hOffScreenDC,
				Lox, m_Owner->m_rtCollide.bottom - (m_Owner->m_rtDesk.bottom / 2) - 5,
				m_pLowerBody->m_rtDesk.right * 2, m_pLowerBody->m_rtDesk.bottom * 2,
				m_pLowerBody->m_pColorBmp->m_hMemDC,
				m_pLowerBody->m_rtSrc.left, m_pLowerBody->m_rtSrc.top,
				m_pLowerBody->m_rtSrc.right, m_pLowerBody->m_rtSrc.bottom,
				m_BlendFunction);


			//AlphaBlend(g_hOffScreenDC,
			//	Upx, m_Owner->m_rtCollide.top + (m_pLowerBody->m_rtDesk.bottom / 2),
			//	m_pUpperBody->m_rtDesk.right * 2, m_pUpperBody->m_rtDesk.bottom * 2,
			//	m_pUpperBody->m_pColorBmp->m_hMemDC,
			//	m_pUpperBody->m_rtSrc.left, m_pUpperBody->m_rtSrc.top,
			//	m_pUpperBody->m_rtSrc.right, m_pUpperBody->m_rtSrc.bottom,
			//	m_BlendFunction);

			AlphaBlend(g_hOffScreenDC,
				Upx, m_Owner->m_rtCollide.bottom - ((m_Owner->m_rtCollide.bottom - m_Owner->m_rtCollide.top) / 2) - y + 15,
				m_pUpperBody->m_rtDesk.right * 2, m_pUpperBody->m_rtDesk.bottom * 2,
				m_pUpperBody->m_pColorBmp->m_hMemDC,
				m_pUpperBody->m_rtSrc.left, m_pUpperBody->m_rtSrc.top,
				m_pUpperBody->m_rtSrc.right, m_pUpperBody->m_rtSrc.bottom,
				m_BlendFunction);
		}
		else
		{
			AlphaBlend(g_hOffScreenDC,
				x, m_Owner->m_rtCollide.top,
				Body->m_rtDesk.right * 2, Body->m_rtDesk.bottom * 2,
				Body->m_pColorBmp->m_hMemDC,
				Body->m_rtSrc.left, Body->m_rtSrc.top,
				Body->m_rtSrc.right, Body->m_rtSrc.bottom,
				m_BlendFunction);
		}
	}
		return true;
}
bool sAnimation::SetP(sPoint a)
{
	pos = a;
	return 0;
}

bool sAnimation::Completion()
{
	if (Body != nullptr)
	{
		if (Body->m_rtList.size() <= Body->m_iRectIndex) return true;	
	}
	if (m_pLowerBody->m_rtList.size() <= m_pLowerBody->m_iRectIndex || m_pUpperBody->m_rtList.size() <= m_pUpperBody->m_iRectIndex)
	{
		return true;
	}
	return false;
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