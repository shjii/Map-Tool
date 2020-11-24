#pragma once
#include "sScene.h"
#include "sInPut.h"

class sSceneIntro :
	public sScene
{
	bool	Frame()
	{
		sScene::Frame();
		if (sCollision::RectInpt(m_objList[1]->m_rtDesk , g_InPut.GetPos()))
		{
			if (g_KeyMap.bLeftClick >= 3)
			{
				m_bSceneChange = true;
			}
		}
		return true;
	}
};

