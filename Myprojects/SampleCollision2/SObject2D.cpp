#include "SObject2D.h"
bool	SObject2D::Create(ID3D11Device* pDevice, T_STR szVS, T_STR szPS, T_STR	szTex)
{
	if (!SShape::Create(pDevice,szVS,szPS,szTex))
	{
		return false;
	}
	InitBufferData(pDevice);
	return true;
}
bool	SObject2D::CreateInputLayout()
{
	return true;
}
bool	SObject2D::SetMatrix(Matrix* pWorld, Matrix* pView, Matrix* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
		m_vCenter.x = pWorld->_41;
		m_vCenter.y = pWorld->_42;
		m_vCenter.z = pWorld->_43;
	}
	if (pView)
	{
		m_matView = *pView;
	}
	if (pProj)
	{
		m_matProj = *pProj;
	}

	m_matBillboard = m_matView.Invert();
	m_matBillboard._41 = 0.0f;
	m_matBillboard._42 = 0.0f;
	m_matBillboard._43 = 0.0f;
	m_matBillboard._44 = 1.0f;
	m_matWorld = m_matBillboard * m_matWorld;
	SShape::SetMatrix(&m_matWorld, &m_matView, &m_matProj);
	return true;
}
bool	SObject2D::Frame(ID3D11DeviceContext* pContext, float fGlobalTime, float fElapsdTime)
{

	return true;
}
HRESULT	SObject2D::InitBufferData(ID3D11Device* pd3dDevice)
{
	HRESULT hr;
	srand(timeGetTime());
	Matrix matTrans;
	if (m_iCounter < 1) m_iCounter = MAX_PARTICLE;
	m_ParticleList.reserve(m_iCounter);
	m_ParticleList.resize(m_iCounter);

	SAFE_NEW_ARRAY(m_pInstatnceList, SInstance, m_ParticleList.size());
	for (int iPlane = 0; iPlane < m_ParticleList.size(); iPlane++)
	{
		SParticle particle;
		matTrans = XMMatrixScaling(particle.m_vPos.x, particle.m_vPos.y, particle.m_vPos.z);
		m_pInstatnceList[iPlane].mMatWorld = matTrans;
		m_pInstatnceList[iPlane].vColor = particle.m_vColor;
		m_ParticleList[iPlane] = particle;
	}
	m_BufferInstance.Attach(TBASIS_CORE_LIB::CreateVertexBuffer(pd3dDevice, m_pInstatnceList, (UINT)m_ParticleList.size(), sizeof(SInstance), true));
	return S_OK;
}
SObject2D::SObject2D()
{

}
SObject2D::~SObject2D()
{

}