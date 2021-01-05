#include "sWrite.h"
#include "sTime.h"

void sWrite::push(sMsg msg)
{
	if (m_TextList.size() > 20)
	{
		m_TextList.pop_back();
	}
	m_TextList.insert(
		m_TextList.begin(),
		msg);
}
void   sWrite::chpush(sMsg msg)
{
	if (msg.msg == L"\r")
	{
		keyc = true;
		
	}
	else if (msg.msg == L"\b")
	{
		if (m_chText.msg.empty() == false)
		{
			m_chText.msg = m_chText.msg.erase(m_chText.msg.length() - 1, m_chText.msg.length());
		}
		
	}else
	{
		m_chText.msg += msg.msg;
		//m_chText.msg.clear();
	}
}
bool sWrite::Init()
{
	m_hFont = CreateFont(20, 0, 0, FW_BOLD, 0, 0, 0, 0,
		DEFAULT_CHARSET, 0, 0, 0, 0,
		L"고딕");
	//SelectObject 같은 경우 기존 값을 반환함
	SelectObject(g_hOffScreenDC, m_hFont);
	return true;
}
bool	sWrite::chc(wstring& a) 
{
	if (keyc)
	{
		keyc = false;
		a = m_chText.msg;
		m_chText.msg.clear();
		return true;
	}
	return false;
}

bool sWrite::Frame()
{
	return true;
}

bool sWrite::Render()
{
	if (g_hOffScreenDC != NULL)
	{
		//RECT rect;
		//GetClientRect(g_hWnd, &rect);
		SetBkColor(g_hOffScreenDC, RGB(255, 0, 0));
		SetTextColor(g_hOffScreenDC, RGB(0, 0, 255));
		SetBkMode(g_hOffScreenDC, TRANSPARENT);
		for (int iMsg = 0; iMsg < m_TextList.size(); iMsg++)
		{
			m_TextList[iMsg].rt.top = 25 + iMsg * 25;
			DrawText(g_hOffScreenDC,
				m_TextList[iMsg].msg.c_str(),
				-1,
				&m_TextList[iMsg].rt,
				DT_LEFT);
		}

		m_chText.rt = { 0,550,g_rtClient.right, g_rtClient.bottom };
		DrawText(g_hOffScreenDC,
			m_chText.msg.c_str(),
			-1,
			&m_chText.rt,
			DT_LEFT);

	}
	return true;
}

void sWrite::Draw(int x, int y, wstring msg, DWORD color)
{
	POINT p = { x,y };
	Draw(p, msg, color);
}
void sWrite::Draw(POINT pos, wstring msg, DWORD color)
{
	if (g_hOffScreenDC != NULL)
	{
		SetBkColor(g_hOffScreenDC, RGB(255, 0, 0));
		SetTextColor(g_hOffScreenDC, color);
		SetBkMode(g_hOffScreenDC, TRANSPARENT);
		TextOut(g_hOffScreenDC, pos.x, pos.y, msg.c_str(), msg.size());
	}
}

bool sWrite::Release()
{
	DeleteObject(m_hFont);
	return true;
}

sWrite::sWrite()
{

}
sWrite::~sWrite()
{

}