#pragma once
#include "KDevice.h"
#include "KTimer.h"
#include "KInPut.h"
#include "KWrite.h"
#include "KSoundManager.h"
#include "KTextureManager.h"
class KCore : public KDevice
{
public:
	bool				m_bGameRun;
public:
	virtual bool	PreInit();
	virtual bool	Init();
	virtual bool	PostInit();
	virtual bool	PreFrame() {return true;};
	virtual bool	Frame() {return true;};
	virtual bool	PostFrame() {return true;};
	virtual void    CameraFrame();
	virtual bool	Render() {return true;};
	virtual bool	PreRender();
	virtual bool	PostRender();
	virtual bool	Release() {return true;	};
	virtual HRESULT DeleteDXResource();
	virtual HRESULT CreateDXResource(UINT w, UINT h);
	bool	InitTool(HWND hwnd, HINSTANCE hlnstance);
	bool	ReleaseTool();
	bool	ToolRun();
private:
	bool	GameInit();
	bool	GameFrame();
	bool	GameRender();
	bool	GameRelease();
	bool	GameRun();
public:
	bool	Run();
public:
	KCore();
	virtual ~KCore();
};

