#pragma once
#include "sStd.h"
// 1.  ���� ���� �ð� ���� ���
// 2. 1������ ��� �ð� ����
// 3. 1�� ���� ������ ī����
class sTime
{
public:
	// ���� ���� �ð� ���� �ϴ� �Լ�
	float	m_fGameTimer;
	// 1������ ��� �ð� �����ϴ� �Լ�
	float	m_fSecondPerFrame;
	// 1�� ���� ������ ī���� �ϴ� �Լ�
	int		m_iFPS;

	float fBeforeTime;
public:
	//�ʱ�ȭ �� �غ� �۾� ����
	bool	Init();
	// ������ ������ ����۾�
	bool	Frame();
	// ������ ������ ������
	bool	Render();
	// �Ҹ� �� ���� ��� ����
	bool	Release();

};

