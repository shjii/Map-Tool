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
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "mTransform",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "mTransform",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "mTransform",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "mTransform",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "mTransform",  0, DXGI_FORMAT_R32G32_FLOAT, 1, 48,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
	HRESULT hr = g_pd3dDevice->CreateInputLayout(
		layout,
		iNumElement,
		m_pVSObj->GetBufferPointer(),
		m_pVSObj->GetBufferSize(),
		m_pInputLayout.GetAddressOf()
	);
	if (FAILED(hr)) return false;
	return true;
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
	if (!SShape::Frame()) return false;



	if (m_BufferInstance)
	{
		SInstance* pInstances = NULL;
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		if (SUCCEEDED(pContext->Map((ID3D11Resource*)m_BufferInstance.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0 , &MappedFaceDest)))
		{
			pInstances = (SInstance*)MappedFaceDest.pData;
			memcpy(pInstances, m_pInstatnceList, sizeof(SInstance) * m_ParticleList.size());
			pContext->Unmap(m_BufferInstance.Get(), 0);
		}
	}
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
bool	SObject2D::Render(ID3D11DeviceContext* pContext)
{
	ID3D11Buffer* Buffer[2] = { m_pVertexBuffer.Get(), m_BufferInstance.Get() };
	UINT stride[2] = { sizeof(PNCT_VERTEX), sizeof(SInstance) };
	UINT offset[2] = { 0,0 };

	PreRender(pContext);
	pContext->IASetVertexBuffers(0,2,Buffer,stride, offset);
	pContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, NULL, &m_cbData,0 ,0);
	pContext->DrawIndexedInstanced(m_iIndex, m_ParticleList.size(), 0, 0, 0);
	// ¼öÁ¤
	return true;
}
SObject2D::SObject2D()
{

}
SObject2D::~SObject2D()
{

}