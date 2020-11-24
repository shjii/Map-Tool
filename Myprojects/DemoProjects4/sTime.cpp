#include "sTime.h"

float	 g_fGameTimer = 0.0f;
float    g_fSecondPerFrame = 0.0f;

//초기화 및 준비 작업 구현
bool sTime::Init()
{
	m_fGameTimer = 0.0f;
	m_fSecondPerFrame = 0.0f;
	m_iFPS = 0;
	fBeforeTime = timeGetTime();
	return true;
}
// 프레임 단위로 계산작업
bool sTime::Frame()
{
	float fCurrentTime = timeGetTime();
	m_fSecondPerFrame = (fCurrentTime - fBeforeTime) / 1000.0f;
	m_fGameTimer += m_fSecondPerFrame;
	fBeforeTime = fCurrentTime;

	g_fGameTimer = m_fGameTimer;
	g_fSecondPerFrame = m_fSecondPerFrame;
	return true;
}
// 프레임 단위로 렌더링
bool sTime::Render()
{
	static float fTimer = 0.0f;
	fTimer += m_fSecondPerFrame;
	if (fTimer >= 1.0f)
	{
		ZeroMemory(m_szBuffer, sizeof(WCHAR) * 256);
		_stprintf_s(m_szBuffer, L"\n GameTimer = %10.4f,spf(%10.4f), fps(%d)",
			m_fGameTimer,
			m_fSecondPerFrame
			, m_iFPS);
		OutputDebugString(m_szBuffer);
		fTimer -= 1.0f;
		m_iFPS = 0;
	}
	m_iFPS++;
	return true;
}
// 소멸 및 삭제 기능 구현
bool sTime::Release()
{
	return true;
}