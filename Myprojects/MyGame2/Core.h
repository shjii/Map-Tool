#pragma once
#include "myWin.h"

class Core:public myWin
{
public:
	
	bool m_bGameRun;
public:
	virtual bool Init() { return false; }
	virtual bool Frame() { return false; }
	virtual bool Render() { return false; }
	virtual bool Release() { return false; }
private:
	bool GameInit();
	bool GameRelease();
	bool GameRun();
public:
	bool Run();
	
};