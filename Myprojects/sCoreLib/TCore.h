#pragma once
#include "TDevice.h"
#include "TTimer.h"
#include "TInput.h"
#include "TWrite.h"
#include "TSoundManager.h"
#include "SDebugCamera.h"
class TCore : public TDevice
{
public:
	bool				m_bGameRun;
	SDebugCamera	    m_Camera;
	SCamera*		    m_pMainCamera;
	//SDebugCamera*	    m_pMainCamera;
	SShapeLine			m_LineShape;
public:
	virtual bool	PreInit();
	virtual bool	Init();
	virtual bool	PostInit();

	virtual bool	PreFrame() {
		return true;
	};
	virtual void    CameraFrame();
	virtual bool	Frame() {
		return true;
	};
	virtual bool	PostFrame() {
		return true;
	};
	virtual bool	Render() {
		return true;
	};
	virtual bool	PreRender();
	virtual bool	PostRender();

	virtual bool	Release() {
		return true;
	};
	virtual HRESULT DeleteDXResource();
	virtual HRESULT CreateDXResource(UINT w, UINT h);
private:
	bool	GameInit();
	bool	GameFrame();
	bool	GameRender();
	bool	GameRelease();
	bool	GameRun();
public:
	bool	Run();
public:
	TCore();
	virtual ~TCore();
};

