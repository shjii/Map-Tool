#include "sTime.h"

//�ʱ�ȭ �� �غ� �۾� ����
bool sTime::Init()
{
	m_fGameTimer = 0.0f;
	m_fSecondPerFrame = 0.0f;
	m_iFPS = 0;
	fBeforeTime = timeGetTime();
	return true;
}
// ������ ������ ����۾�
bool sTime::Frame()
{
	float fCurrentTime = timeGetTime();
	m_fSecondPerFrame = (fCurrentTime - fBeforeTime) / 1000.0f;
	m_fGameTimer += m_fSecondPerFrame;
	fBeforeTime = fCurrentTime;
	return true;
}
// ������ ������ ������
bool sTime::Render()
{
	static float fTimer = 0.0f;
	fTimer += m_fSecondPerFrame;
	if (fTimer >= 1.0f)
	{
		char szBuffer[256] = { 0, };
		sprintf_s(szBuffer, "\n GameTimer = %10.4f,spf(%10.4f), fps(%d)",
			m_fGameTimer,
			m_fSecondPerFrame
			, m_iFPS);
		OutputDebugStringA(szBuffer);
		fTimer -= 1.0f;
		m_iFPS = 0;
	}
	m_iFPS++;
	return true;
}
// �Ҹ� �� ���� ��� ����
bool sTime::Release()
{
	return true;
}