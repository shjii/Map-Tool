#pragma once
#include "sCore.h"
#include  "sSceneInGame.h"
#include "sSceneIntro.h"
#include "sSceneLobby.h"


class main : public sCore
{
public:
	sScene* m_pCurrentScene;
	vector< sScene*> m_SceneList;
	int m_iCurrentScene;
public:
	sPoint m_ptInitHeroPos;
	float m_fSpeed;
public:
	bool	Init()		;
	bool	Frame()		;
	bool	Render()	;
	bool	Release()	;
public:

};

