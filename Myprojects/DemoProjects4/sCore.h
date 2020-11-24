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
	// ���� �����Ҷ�
	virtual bool	Init() { return true; }
	//������ �������� ����
	virtual bool	Frame() { return true; }
	// ������ ������ ȭ�鿡 ���
	virtual bool	Render() { return true; }

	virtual bool	PreRender();

	virtual bool	PostRender();
	// �Ҹ�, ����
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

