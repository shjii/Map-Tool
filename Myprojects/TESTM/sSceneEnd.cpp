#include "sSceneEnd.h"

bool	sSceneEnd::Frame()
{
	sScene::Frame();
	a += g_fSecondPerFrame;
	if (a >= 2.0f)
	{
		m_bSceneChange == true;
	}
	return true;
}