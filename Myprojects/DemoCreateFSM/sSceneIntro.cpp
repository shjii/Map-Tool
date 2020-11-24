#include "sSceneIntro.h"

bool	sSceneIntro::Frame()
{
	sScene::Frame();
	for (auto obj : m_UIobjList)
	{
		if (obj->m_iObjectType != 301) continue;
		if (sCollision::RectInpt(obj->m_rtDesk,
			g_InPut.GetPos()))
		{
			if (g_KeyMap.bLeftClick == 3)
			{
				m_bSceneChange = true;
			}
		}
	}
	return true;
}