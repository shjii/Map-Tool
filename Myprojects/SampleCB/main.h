#pragma once
#include "TCore.h"
#include "TStd.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
struct SVertex3
{
	float  x;
	float  y;
	float  z;
	SVertex3() {}
	SVertex3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

struct P_VERTEX
{
	SVertex3 p;
	SVertex3 n;
	P_VERTEX() {}
	P_VERTEX(SVertex3 p, SVertex3 n = {0,0,0})
	{
		this->p = p;
		this->n = n;
	}
};
struct SDataCB
{
	float vColor[4];
	float vTime[4];
};
class main : public TCore
{
	SDataCB m_cbData;
	std::vector<P_VERTEX>	m_VertexList;
	std::vector<DWORD>		m_IndexList;
	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11Buffer*			m_pIndexBuffer;
	ID3D11Buffer*			m_pConstantBuffer;
	ID3D11InputLayout*		m_pInputLayout;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;

	ID3D11RasterizerState*	m_pRSSolidBack;
	ID3D11RasterizerState*	m_pRSWireBack;
	ID3D11RasterizerState*	m_pRS;
	D3D11_FILL_MODE			m_FillMode;
	D3D11_CULL_MODE			m_CullMode;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
public:
	void CompilerCheck(ID3DBlob* pErrorMsgs);
	void SetRasterizerState();
};

GAME_RUN;

