#pragma once
#include "sNetworkClient.h"
#include "sCore.h"
class main : public sCore
{
public:
	sNetworkClient	m_Network;
	HANDLE			m_hExecuteMutex;
public:
	bool	Init()		override;
	bool	Frame()		override;
	bool	Render()	override;
	bool	Release()	override;
	void	MsgEvent(MSG msg) override;
public:
	void SendTest();
};

