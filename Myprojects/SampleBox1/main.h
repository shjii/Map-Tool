#pragma once
#include "TCore.h"
#include "SShape.h"

class main : public TCore
{
public:
	SShapeBox		m_Box;
	SShapePlane		m_Plane;
	SShapeLine		m_Line;
public:
	xMatrix    m_matBoxWorld;
	xMatrix    m_matPlaneWorld;
	xMatrix    m_matView;
	xMatrix    m_matProj;
public:
	Vector3	   m_vCameraPos;
	Vector3	   m_vCameraTarget;
public:
	ID3D11RasterizerState*	m_pRSSolidBack;
	ID3D11RasterizerState*	m_pRSWireBack;
	ID3D11RasterizerState*	m_pRS;
	ID3D11SamplerState*		m_pWrapLinear;
	D3D11_FILL_MODE			m_FillMode;
	D3D11_CULL_MODE			m_CullMode;
public:
	ID3D11DepthStencilView*  m_pDSV;
	ID3D11DepthStencilState* m_pDSS;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool PreRender() override;
	bool Release() override;
public:
	void CompilerCheck(ID3DBlob* pErrorMsgs);
	void SetRasterizerState();
};

GAME_RUN;

