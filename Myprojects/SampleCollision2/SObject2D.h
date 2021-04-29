#pragma once
#include "SShape.h"
#include "SParticle.h"

#pragma pack(push, 1)
struct SInstance
{
	Matrix	mMatWorld;
	Vector4	vColor;
};
#pragma pack(pop)
class SObject2D : public SShape
{
public:
	vector<SParticle>		m_ParticleList;
	ComPtr<ID3D11Buffer>	m_BufferInstance;
	SInstance*				m_pInstatnceList;
	int						m_iCounter;
	Matrix					m_matBillboard;
	Vector3					m_vCenter;
public:
	void	SetCounter(int iCnt) { m_iCounter = iCnt; };
public:
	bool	Create(ID3D11Device* pDevice, T_STR szVS, T_STR szPS, T_STR	szTex) override;
	bool	CreateInputLayout()override;
	bool	SetMatrix(Matrix* pWorld, Matrix* pView, Matrix* pProj)override;
	bool	Frame(ID3D11DeviceContext* pContext, float fGlobalTime, float fElapsdTime);
	bool	Render(ID3D11DeviceContext* pContext)override;
	HRESULT	InitBufferData(ID3D11Device* pd3dDevice);
public:
	SObject2D();
	~SObject2D();
};

