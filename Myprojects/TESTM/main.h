#pragma once
#include "sCore.h"
#include "sSceneIntro.h"
#include "sSceneLobby.h"
#include "sSceneInGame.h"
#include "sFiniteStateMachine.h"
#include "sSceneEnd.h"
#include "sSoundManager.h"
class main : public sCore
{
public:
	sSound*			m_pBG;
	sSound*			m_pEffect;
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
