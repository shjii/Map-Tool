#pragma once
#include "sObject.h"
#include "sInPut.h"
class sUserInterface : public sObject
{
public:
	int m_iState;
	sUserInterface() : m_iState(0)
	{}
};


class sButton : public sUserInterface
{
public:
	bool Frame()
	{
		m_iState = 0;
		if (sCollision::RectInpt(m_rtDesk,g_InPut.GetPos()))
		{
			m_iState = 1;
			if (g_KeyMap.bLeftClick > 0)
			{
				m_iState = 2;
			}
		}
		return true;
	}

	bool Render()
	{
		if (m_bColorKey == true)
		{
			switch (m_iState)
			{
			case 0:
			{
				m_pColorBmp->DrawColorKey(
					m_rtDesk,
					m_rtSrc,
					m_dwColorKey);
			}break;
			case 1:
			{
				m_StateBitmap[0]->DrawColorKey(
					m_rtDesk,
					m_rtSrc,
					m_dwColorKey);
			}break;
			case 2:
			{
				m_StateBitmap[1]->DrawColorKey(
					m_rtDesk,
					m_rtSrc,
					m_dwColorKey);
			}break;
			case 3:
			{
				m_StateBitmap[2]->DrawColorKey(
					m_rtDesk,
					m_rtSrc,
					m_dwColorKey);
			}break;
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
			case 1: {m_StateBitmap[0]->Render(); }break;
			case 2: {m_StateBitmap[1]->Render(); }break;
			case 3: {m_StateBitmap[2]->Render(); }break;
			}
		}
		return true;
	}
};
