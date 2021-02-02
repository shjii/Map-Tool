#include "sUserInterface.h"

bool sButton::Frame()
{
	m_iState = 0;
	if (sCollision::RectInpt(m_rtDesk, g_InPut.GetPos()))
	{
		m_iState = 1;
		if (g_KeyMap.bLeftClick > 0)
		{
			m_iState = 2;
		}
	}
	return true;
}

bool sButton::Render()
{
	/*if (m_bColorKey == true)
	{
		sBitmap* pCurrent = m_pColorBmp;
		(m_StateBitmap[m_iState] != nullptr) ?
			pCurrent = m_StateBitmap[m_iState] :
			pCurrent = nullptr;

		if (pCurrent != nullptr)
		{
			pCurrent->DrawColorKey(
				m_rtDesk,
				m_rtSrc,
				m_dwColorKey);
		}
		return true;
	}
	if (m_pColorBmp && m_pColorBmp->m_BitmapInfo.bmBitsPixel == 32)
	{
		switch (m_iState)
		{
		case 0: {m_pColorBmp->DrawAlphaBlend(m_rtDesk, m_rtSrc); }break;
		case 1: {m_StateBitmap[1]->DrawAlphaBlend(m_rtDesk, m_rtSrc); }break;
		case 2: {m_StateBitmap[2]->DrawAlphaBlend(m_rtDesk, m_rtSrc); }break;
		case 3: {m_StateBitmap[3]->DrawAlphaBlend(m_rtDesk, m_rtSrc); }break;
		}
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
		switch (m_iState)
		{
		case 0: {m_pColorBmp->Render(); }break;
		case 1: {m_StateBitmap[1]->Render(); }break;
		case 2: {m_StateBitmap[2]->Render(); }break;
		case 3: {m_StateBitmap[3]->Render(); }break;
		}
	}*/
	return true;
}