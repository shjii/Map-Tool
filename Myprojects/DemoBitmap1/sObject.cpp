#include "sObject.h"


bool	sObject::Init()
{
	m_pMaskBmp = nullptr;
	m_pColorBmp = nullptr;
	return true;
}
bool	sObject::Frame()
{
	return true;

}
bool	sObject::Render()
{
	if (m_bColorKey == true)
	{
		DrawColorKey();
		return true;
	}
	if (m_pMaskBmp != nullptr)
	{
		m_pMaskBmp->Draw(m_rtDesk, m_rtSrc, SRCAND, 1);
		m_pColorBmp->Draw(m_rtDesk, m_rtSrc, SRCINVERT, 1);
		m_pMaskBmp->Draw(m_rtDesk, m_rtSrc, SRCINVERT, 1);
	}
	else
	{
		m_pColorBmp->Render();
	}
	return true;
}
bool	sObject::Release()
{
	return true;
}

void	sObject::SetPosition(sPoint p)
{
	m_ptPos = p;
	m_rtDesk.left = p.x;
	m_rtDesk.top = p.y;
}
void	sObject::Set(RECT rtSrc, RECT rtDesk, float fSpeed)
{
	m_fSpeed = fSpeed;
	if (rtSrc.left >= 0) m_rtSrc.left = rtSrc.left;
	if (rtSrc.top >= 0) m_rtSrc.top = rtSrc.top;
	if (rtSrc.right >= 0) m_rtSrc.right = rtSrc.right;
	if (rtSrc.bottom >= 0) m_rtSrc.bottom = rtSrc.bottom;
	if (rtDesk.left >= 0) m_rtDesk.left = rtDesk.left;
	if (rtDesk.top >= 0) m_rtDesk.top = rtDesk.top;
	if (rtDesk.right >= 0) m_rtDesk.right = rtDesk.right;
	if (rtDesk.bottom >= 0) m_rtDesk.bottom = rtDesk.bottom;
	m_ptPos.x = m_rtDesk.left;
	m_ptPos.y = m_rtDesk.top;
}
bool	sObject::Load(const TCHAR* color, const TCHAR* mask)
{
	if (color != nullptr)
	{
		m_pColorBmp = g_BitMapMar.Load(color);
		if (m_pColorBmp != nullptr)
		{
			Set({ 0,0,m_pColorBmp->m_BitmapInfo.bmWidth, m_pColorBmp->m_BitmapInfo.bmHeight },
				{ 0,0, m_pColorBmp->m_BitmapInfo.bmWidth, m_pColorBmp->m_BitmapInfo.bmHeight });
		}
	}
	int a = wcslen(mask);

	if (a > 0)
	{
		m_pMaskBmp = g_BitMapMar.Load(mask);
	}

	/*if (mask != L'\0')
	{
		m_pMaskBmp = g_BitMapMar.Load(mask);
	}*/
	return true;
}

bool	sObject::Load(const TCHAR* color, DWORD dwColor)
{
	if (color != nullptr)
	{
		m_pColorBmp = g_BitMapMar.Load(color);
		if (m_pColorBmp != nullptr)
		{
			Set({ 0,0,m_pColorBmp->m_BitmapInfo.bmWidth, m_pColorBmp->m_BitmapInfo.bmHeight },
				{ 0,0, m_pColorBmp->m_BitmapInfo.bmWidth, m_pColorBmp->m_BitmapInfo.bmHeight });
		}
	}
	m_bColorKey = true;
	m_dwColorKey = dwColor;

	return true;
}
void	sObject::DrawColorKey()
{
	m_pColorBmp->DrawColorKey(
		m_rtDesk, m_rtSrc, m_dwColorKey
	);
}

sObject::sObject()
{
	m_fSpeed = 200.0f;
	m_bColorKey = false;
	m_dwColorKey = RGB(255,255,255);
}

sObject::~sObject()
{

}