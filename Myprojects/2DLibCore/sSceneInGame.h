#pragma once
#include "sScene.h"
class sSceneInGame :
	public sScene
{
public:
	int stage;
	bool m_bGameFinish;
public:
	bool Frame()override;
	bool Render()override;
	sSceneInGame();
};

