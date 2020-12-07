#pragma once
#include "sNetworkClient.h"
#include "sCore.h"
class main : public sCore
{
public:
	sNetworkClient	m_Network;
	sUser			m_User;
public:
	bool	Init()		override;
	bool	Frame()		override;
	bool	Render()	override;
	bool	Release()	override;
};

