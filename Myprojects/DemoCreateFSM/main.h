#pragma once
#include "sCore.h"
#include "sSceneIntro.h"
#include "sSceneLobby.h"
#include "sSceneInGame.h"
#include "sFiniteStateMachine.h"

class main : public sCore
{
	sScene* m_pCurrentScene;
	vector<sScene*> m_SceneList;
	int m_iCurrenScene;
	sPoint m_pInitHeroPos;
	float m_fSpeed;

public:

	bool	Init()		;
	bool	Frame()		;
	bool	Render()	;
	bool	Release()	;


};
