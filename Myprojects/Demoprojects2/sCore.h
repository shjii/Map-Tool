#pragma once
#include "sWindow.h"

class sCore:public sWindow
{
public:
	bool	m_bGameRun;
public:
	// ���� �����Ҷ�
	virtual bool	Init() { return true; }
	//������ �������� ����
	virtual bool	Frame() { return true; }
	// ������ ������ ȭ�鿡 ���
	virtual bool	Render() { return true; }
	// �Ҹ�, ����
	virtual bool	Release() { return true; }
private:
	bool			GameInit();
	bool			GameRelease();
	bool			GameRun();
public:
	bool			Run();
};

