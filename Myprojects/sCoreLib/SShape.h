#pragma once
#include "TStd.h"
#include "dxtk/WICTextureLoader.h"
#include "SimpleMath.h"
#include "SDxState.h"
using namespace DirectX;
using namespace SimpleMath;

struct P_VERTEX
{
	Vector3 p;
	Vector3 n;
	Vector4 c;
	Vector2 t;
	P_VERTEX() {}
	P_VERTEX(Vector3 p,
		Vector3 n,
		Vector4 c,
		Vector2 t)
	{
		this->p = p;
		this->n = n;
		this->c = c;
		this->t = t;
	}
};

struct PNCT_VERTEX
{
	Vector3 p;
	Vector3 n;
	Vector4 c;
	Vector2 t;

	bool operator == (const PNCT_VERTEX& Vertex)
	{
		if (p == Vertex.p && n == Vertex.n && c == Vertex.c && t == Vertex.t)
		{
			return true;
		}
		return false;
	}

	PNCT_VERTEX() {}
	PNCT_VERTEX(Vector3 p,
		Vector3 n,
		Vector4 c,
		Vector2 t)
	{
		this->p = p;
		this->n = n;
		this->c = c;
		this->t = t;
	}
};

struct SDataCB
{
	Matrix  matWorld;
	Matrix  matView;
	Matrix  matProject;
	float vColor[4];
	float vTime[4];
};

class SShape
{
public:
	ID3D11Device*	m_pd3dDevice;
	ID3DBlob*		m_pVSobj;
	UINT			m_iTopology;
	C_STR			m_szVertexShader;
	C_STR			m_szPixelShader;
public:
	Matrix    m_matWorld;
	Matrix    m_matView;
	Matrix    m_matProj;
public:
	SDataCB					m_cbData;
	std::vector<PNCT_VERTEX>	m_VertexList;
	std::vector<DWORD>		m_IndexList;
	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11Buffer*			m_pIndexBuffer;
	ID3D11Buffer*			m_pConstantBuffer;
	ID3D11InputLayout*		m_pInputLayout;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11ShaderResourceView* m_pTextureSRV;
public:
	void CompilerCheck(ID3DBlob* pErrorMsgs);
	virtual bool SetMatrix(Matrix* pWorld, Matrix* pView, Matrix* pProj);
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render(ID3D11DeviceContext* pd3dContext);
	virtual bool Release();
	virtual bool CreateVertexData();
	virtual bool CreateConstantBuffer();
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexData();
	virtual bool CreateIndexBuffer();
	virtual bool LoadShader(T_STR szVS, T_STR szPS);
	virtual bool CreateInputLayout();
	virtual bool Create(ID3D11Device* pDvice, T_STR szVS, T_STR szPS, T_STR szTex);
	virtual bool LoadTexture(T_STR szTex);
	virtual bool Update(ID3D11DeviceContext* pd3dContext);
public:
	SShape();
	virtual ~SShape();
};

class SShapeBox : public SShape
{
public:
	virtual bool CreateVertexData() override;
	virtual bool CreateIndexData() override;
public:
	SShapeBox();
	virtual ~SShapeBox();
};

class SShapePlane : public SShape
{
public:
	virtual bool CreateVertexData() override;
	virtual bool CreateIndexData() override;
public:
	SShapePlane();
	virtual ~SShapePlane();
};

class SShapeLine : public SShape
{
public:
	bool Draw(ID3D11DeviceContext* pd3dContext, Vector3 p, Vector3 e, Vector4 c = Vector4(1,0,0,1));
public:
	virtual bool CreateVertexData() override;
	virtual bool CreateIndexData() override;
public:
	SShapeLine();
	virtual ~SShapeLine();
};