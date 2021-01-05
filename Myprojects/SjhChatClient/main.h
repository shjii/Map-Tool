#pragma once
#include "sNetworkClient.h"
#include "sCore.h"
#include "sFiniteStateMachine.h"
class main : public sCore
{
public:
	wstring IP;
	wstring name;
	int						m_iCurrentScene;
	float fTime;
	wstring a;
public:
	sNetworkClientEventSelect	m_Network;
	HANDLE						m_hExecuteMutex;
	bool						m_bUseNet;
	std::string					m_szName;
public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();
public:
	bool  NetInit();
	bool  NetFrame();
	bool  NetRender();
	bool  NetRelease();
	void  MsgEvent(MSG msg) override;

	void  PlayDataSend();
public:
	void  SendTest(const char*pData);
};

