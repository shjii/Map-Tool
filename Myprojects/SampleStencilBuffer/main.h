#pragma once
#include "TCore.h"
#include "SShape.h"
#include "TObject.h"
#include "SModelViewCamera.h"
#include "SMap.h"
class main : public TCore
{
public:
	SMap			m_Map;
	Vector4			m_vDirValue;
	std::vector<TObject*> m_ObjList;
	SShapeBox		m_BoxShape;
	SShapePlane		m_PlaneShape;
	SShapeLine		m_LineShape;
	SModelViewCamera  m_ModelCamera;
public:
	Matrix			m_matBoxWorld;
	Matrix			m_matPlaneWorld;
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

TGAME_RUN;