#pragma once
#include "sScene.h"
class sSceneInGame :
	public sScene
{
public:
	bool m_bGameFinish;
public:
	bool Frame()override;
	bool Render()override;
	sSceneInGame();
};

