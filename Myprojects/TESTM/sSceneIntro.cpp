#include "sSceneIntro.h"

bool	sSceneIntro::Frame()
{
	sScene::Frame();
	if (g_KeyMap.bAKey != 0 || g_KeyMap.bDKey != 0 || g_KeyMap.bWKey != 0 || g_KeyMap.bSKey != 0 || g_KeyMap.bSpace != 0)
	{
		iNextScene = 1;
		m_bSceneChange = true;
	}
	
	//for (auto obj : m_UIobjList)
	//{
	//	if (obj->m_iObjectType != 301) continue;
	//	if (sCollision::RectInpt(obj->m_rtDesk,
	//		g_InPut.GetPos()))
	//	{
	//		if (g_KeyMap.bLeftClick == 3)
	//		{
	//			m_bSceneChange = true;
	//		}
	//	}
	//}
	return true;
}
bool sSceneIntro::ReSet()
{
	m_bSceneChange = false;
	return true;
}