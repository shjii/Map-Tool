#pragma once
#include "TCore.h"
#include "SShape.h"
#include "TObject.h"
#include "SModelViewCamera.h"
#include "SHeightMap.h"
#include "SMiniMap.h"
#include "SQuadTree.h"
#include "SMousePicking.h"
#include "SCollision.h"
class main : public TCore
{
public:
	bool			BoolColl;
	SCollision		Collision;
	SMiniMap		m_MinMap;
	SCamera			m_TopCamera;
	/*S_BOX			m_TBoxBase;
	SBoxUser		m_UserShape;
	SBoxUser		m_BoxShape;
	SShapePlane		m_PlaneShape;
	std::vector<TObject*> m_ObjList;
	SModelViewCamera  m_ModelCamera;
	*/
	vector<SNode*>	m_SelectNode;
	SMousePicking	m_Mouse;
	/*SHeightMap		m_Map;*/
	SMap			m_Map;
	Vector4			m_vDirValue;
	SQuadTree		m_QuadTree;
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
	void Draw(SNode* Node, ID3D11DeviceContext* pd3dContext);
	//LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
TGAME_RUN;