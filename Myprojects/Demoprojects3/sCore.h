#pragma once
#include "sWindow.h"
#include "sTime.h"
#include "sInPut.h"
class sCore:public sWindow
{
public:
	bool			m_bGameRun;
	sTime			m_sTime;
public:
	// 게임 시작할때
	virtual bool	Init() { return true; }
	//프레임 단위에서 실행
	virtual bool	Frame() { return true; }
	// 프레임 단위로 화면에 출력
	virtual bool	Render() { return true; }
	// 소멸, 삭제
	virtual bool	Release() { return true; }
private:
	bool			GameInit();
	bool			GameRelease();
	bool			GameRun();
public:
	bool			Run();
};

