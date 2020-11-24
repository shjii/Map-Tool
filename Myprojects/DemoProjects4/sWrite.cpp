#include "sWrite.h"

void sWrite::push(sMsg msg)
{
	if (m_TextList.size() > 10)
	{
		m_TextList.pop_back();
	}
	m_TextList.insert(m_TextList.begin(), msg);
}

bool sWrite::Init()
{
	m_hFont = CreateFont(30, 0, 0, FW_BOLD, 0, 0, 0, 0,
		DEFAULT_CHARSET, 0, 0, 0, 0,
		L"고딕");
	//SelectObject 같은 경우 기존 값을 반환함
	SelectObject(g_hOffScreenDC, m_hFont);
	return true;
}
bool sWrite::Render()
{
	if (g_hOffScreenDC != NULL)
	{
		SetBkColor(g_hOffScreenDC, RGB(255, 0, 0));
		SetTextColor(g_hOffScreenDC, RGB(0, 0, 255));
		SetBkMode(g_hOffScreenDC, TRANSPARENT);
		for (int iMsg = 0; iMsg < m_TextList.size(); iMsg++)
		{
			DrawText(g_hOffScreenDC, m_TextList[iMsg].msg.c_str(),
				-1, &m_TextList[iMsg].rt, DT_LEFT);

			//TextOut( DC , x, y, 데이터 , 사이즈);
		}
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
