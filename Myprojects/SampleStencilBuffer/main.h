#pragma once
#include "TCore.h"
#include "SShape.h"
#include "TObject.h"
#include "SModelViewCamera.h"
class SGameObject : public TObject
{
public:
	SShape* m_pShape;
public:
	SGameObject() {};
	virtual ~SGameObject() {};
};

class main : public TCore
{
public:
	Vector4			m_vDirValue;
	SGameObject     m_BoxObj;
	SShapeBox		m_BoxShape;
	SShapePlane		m_PlaneShape;
	SShapeLine		m_LineShape;
	SModelViewCamera  m_ModelCamera;
public:
	Matrix    m_matBoxWorld;
	Matrix    m_matPlaneWorld;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	Matrix CreateMatrixShadow(
		Vector4* plane,
		Vector4* Light);
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

GAME_RUN;

