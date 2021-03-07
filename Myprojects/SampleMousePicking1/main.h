#pragma once
#include "TCore.h"
#include "SShape.h"
#include "TObject.h"
#include "SModelViewCamera.h"
#include "SHeightMap.h"
#include "SMiniMap.h"
#include "SQuadTree.h"
#include "SMousePicking.h"
class main : public TCore
{
public:
	SMiniMap		m_MinMap;
	SCamera			m_TopCamera;
	/*S_BOX			m_TBoxBase;
	SBoxUser		m_UserShape;
	SBoxUser		m_BoxShape;
	SShapePlane		m_PlaneShape;
	std::vector<TObject*> m_ObjList;
	SModelViewCamera  m_ModelCamera;
	*/
	SHeightMap		m_Map;
	Vector4			m_vDirValue;
	SQuadTree		m_QuadTree;
	SMousePicking   m_Mouse;
//public:
//	Matrix			m_matBoxWorld;
//	Matrix			m_matPlaneWorld;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	bool PostRender()override;
	Matrix CreateMatrixShadow(
		Vector4* plane,
		Vector4* Light);
	//LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
TGAME_RUN;