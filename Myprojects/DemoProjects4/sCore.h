#pragma once
#include "sWindow.h"
#include "sTime.h"
#include "sInPut.h"
#include "sGraphicAPI.h"
class sCore:public sWindow
{
public:
	bool			m_bGameRun;
	sGraphicAPI		m_Graphic;
public:
	// 게임 시작할때
	virtual bool	Init() { return true; }
	//프레임 단위에서 실행
	virtual bool	Frame() { return true; }
	// 프레임 단위로 화면에 출력
	virtual bool	Render() { return true; }

	virtual bool	PreRender();

	virtual bool	PostRender();
	// 소멸, 삭제
	virtual bool	Release() { return true; }



private:
	bool	GameInit();
	bool	GameFrame();
	bool	GameRender();
	bool	GameRelease();
	bool	GameRun();
public:
	bool	Run();
};

