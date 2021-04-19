#pragma once
#include "TCore.h"
#include "SShape.h"
#include "TObject.h"
#include "SModelViewCamera.h"
#include "SHeightMap.h"
#include "SMiniMap.h"
#include "SSkyBox.h"
const int g_iNumLight = 3;
struct LIGHT_CONSTANT_BUFFER1
{
	// Material
	Vector4			g_cAmbientMaterial[g_iNumLight];
	Vector4			g_cDiffuseMaterial[g_iNumLight];
	Vector4			g_cSpecularMaterial[g_iNumLight];
	Vector4			g_cEmissionMaterial[g_iNumLight];
	// Light
	Vector4			g_cAmbientLightColor[g_iNumLight];
	Vector4			g_cDiffuseLightColor[g_iNumLight];
	Vector4			g_cSpecularLightColor[g_iNumLight];
};
struct LIGHT_CONSTANT_BUFFER2
{
	Matrix			g_matInvWorld[g_iNumLight];
	Vector4			g_vLightDir[g_iNumLight]; // w = light damping(attenuation) : 감쇠
	Vector4			g_vLightPos[g_iNumLight]; // w = light radius
	Vector4			g_vEyeDir[g_iNumLight];// w = light intensity : 강도
	Vector4			g_vEyePos[g_iNumLight];// w = light radius		
};
struct LIGHT_CONSTANT_BUFFER3
{
	Vector4			g_vSpotInfo[g_iNumLight];
	//float			g_fFalloff;		// 내부 콘과 외부 콘의 휘도( Luminance )
	//float			g_fTheta;		// 내부 콘의 각도 범위		
	//float			g_fPhi;			// 외부 콘의 각도 범위
};
class main : public TCore
{
public:
	SSkyBox			m_SSkyBox;
public:
	Vector3			m_vLightVector;
	LIGHT_CONSTANT_BUFFER1	m_cbLight1;
	LIGHT_CONSTANT_BUFFER2	m_cbLight2;
	LIGHT_CONSTANT_BUFFER3	m_cbLight3;
	ComPtr<ID3D11Buffer>	m_pConstantBufferLight[g_iNumLight];
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