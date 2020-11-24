#pragma once
#include "sScene.h"
class sSceneInGame :
	public sScene
{
public:
	bool m_CockData;
public:
	bool Frame();
	sSceneInGame();
};

