#pragma once
#include "TCore.h"
#include "SShape.h"
#include "TObject.h"
#include "SModelViewCamera.h"
#include "SHeightMap.h"
#include "SMiniMap.h"
class SBoxUser : public SShapeBox
{
public:
	void FrontMovement(float fDir)
	{
		Vector3 vOffset = m_vLook * g_fSecondPerFrame * m_fSpeed * fDir;
		m_vPos += vOffset;
	}
	void RightMovement(float fDir)
	{
		Vector3 vMove = m_vRight * g_fSecondPerFrame * m_fSpeed * fDir;
		m_vPos += vMove;
	}
	void UpMovement(float fDir)
	{
		Vector3 vMove = m_vUp * g_fSecondPerFrame * m_fSpeed * fDir;
		m_vPos += vMove;
	}
	void FrontBase(float fDir)
	{
		Vector3 vSide = { 0,0,1 };
		Vector3 vMove = vSide * g_fSecondPerFrame * m_fSpeed * fDir;
		m_vPos += vMove;
		m_vTarget += m_vLook * m_fSpeed;
	}
	void RightBase(float fDir)
	{
		Vector3 vSide = { 1,0,0 };
		Vector3 vMove = vSide * g_fSecondPerFrame * m_fSpeed * fDir;
		m_vPos += vMove;
		m_vTarget += m_vLook * m_fSpeed;
	}
	void UpBase(float fDir)
	{
		Vector3 vUp = { 0,1,0 };
		Vector3 vMove = vUp * g_fSecondPerFrame * m_fSpeed * fDir;
		m_vPos += vMove;
		m_vTarget += m_vLook * m_fSpeed;
	}
};
class main : public TCore
{
public:
	SMiniMap		m_MinMap;
	SHeightMap		m_Map;
	Vector4			m_vDirValue;
	std::vector<TObject*> m_ObjList;
	SBoxUser		m_BoxShape;
	SShapePlane		m_PlaneShape;
	SShapeLine		m_LineShape;
	SModelViewCamera  m_ModelCamera;
	SCamera			m_TopCamera;
public:
	Matrix			m_matBoxWorld;
	Matrix			m_matPlaneWorld;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	bool PostRender()override;
	Matrix CreateMatrixShadow(
		Vector4* plane,
		Vector4* Light);
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

TGAME_RUN;