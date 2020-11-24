#pragma once
#include "sScene.h"
class sSceneInGame :
	public sScene
{
public:
	int stage;
	WCHAR m_szBuffer[256] = {0,};
	bool m_bGameFinish;
public:
	bool ReSet()override;
	bool Frame()override;
	bool Render()override;
	sSceneInGame();
};

