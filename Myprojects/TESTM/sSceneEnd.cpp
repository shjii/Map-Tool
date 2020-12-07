#include "sSceneEnd.h"

bool	sSceneEnd::Frame()
{
	sScene::Frame();
	a += g_fSecondPerFrame;
	if (a >= 2.0f)
	{
		m_bSceneChange = true;
		a = 0;
		iNextScene = 0;
	}
	return true;
}

bool sSceneEnd::ReSet()
{
	m_bSceneChange = false;
	return true;
}