#pragma once
#include "sNetworkClient.h"
#include "sCore.h"
class main : public sCore
{
public:
	sNetworkClientEventSelect	m_Network;
	HANDLE						m_hExecuteMutex;
	bool						m_bUseNet;
	string						m_szName;
public:
	bool	Init()		;
	bool	Frame()		;
	bool	Render()	;
	bool	Release()	;
	void	MsgEvent(MSG msg) override;
public:
	void SendTest(const char* pData);
};

