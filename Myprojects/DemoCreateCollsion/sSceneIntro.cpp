#include "sSceneIntro.h"

bool	sSceneIntro::Frame()
{
	sScene::Frame();
	if (sCollision::RectInpt(m_UIobjList[0]->m_rtDesk,
		g_InPut.GetPos()))
	{
		if (g_KeyMap.bLeftClick == 3)
		{
			m_bSceneChange = true;
		}
	}
	return true;
}