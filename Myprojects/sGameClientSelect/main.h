#pragma once
#include "sNetworkClient.h"
#include "sCore.h"
class main : public sCore
{
public:
	sNetworkClient	m_Network;
	HANDLE			m_hExecuteMutex;
public:
	bool	Init()		;
	bool	Frame()		;
	bool	Render()	;
	bool	Release()	;
	void	MsgEvent(MSG msg) override;
public:
	void SendTest();
};

