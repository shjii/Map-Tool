#include "pch.h"
#include "Sample.h"

bool Sample::Init()
{
	m_pObj.Init();
	m_pObj.m_pMainCamera = m_pMainCamera;
	return true;
}
bool Sample::Frame()
{
	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		SDxState::m_FillMode = D3D11_FILL_WIREFRAME;
		SDxState::SetRasterizerState(g_pd3dDevice);
	}
	if (g_Input.GetKey('9') == KEY_PUSH)
	{
		SDxState::m_FillMode = D3D11_FILL_SOLID;
		SDxState::SetRasterizerState(g_pd3dDevice);
	}
	if (m_Map != nullptr)
	{
		m_QuadTree.Frame();
		m_Map->Frame();
	}	
	if (g_Input.GetKey(VK_RBUTTON) == KEY_HOLD)
	{
		m_Mouse.RayFrame(m_pMainCamera->m_matWorld, m_pMainCamera->m_matView, m_pMainCamera->m_matProj);
		BoolColl = true;
	}
	m_pObj.Frame();
	return true;
}
bool Sample::Render()
{

	for (auto data : m_MatrixList)
	{
		m_pObj.m_Obj->SetMatrix(&data, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_pObj.Render(g_pImmediateContext);
	}

	if (m_Map != nullptr) {
		g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_pImmediateContext->RSSetState(SDxState::m_pRS);
		g_pImmediateContext->PSSetSamplers(0, 1, &SDxState::m_pWrapLinear);
		g_pImmediateContext->OMSetDepthStencilState(SDxState::m_pDSS, 0);

		m_Map->SetMatrix(NULL,
			&m_pMainCamera->m_matView,
			&m_pMainCamera->m_matProj);


			m_Map->SDxObject::Update(g_pImmediateContext);

			ID3D11Buffer* vb[2] = { m_Map->m_pVertexBuffer.Get(),	m_Map->m_pRC.Get() };
			UINT stride[2] = { sizeof(PNCT_VERTEX), sizeof(RC) };
			UINT offset[2] = { 0, 0 };
			g_pImmediateContext->IASetVertexBuffers(0, 2, vb, stride, offset);
			g_pImmediateContext->IASetIndexBuffer(m_Map->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			g_pImmediateContext->IASetInputLayout(m_Map->m_pInputLayout.Get());
			g_pImmediateContext->VSSetConstantBuffers(0, 1, m_Map->m_pConstantBuffer.GetAddressOf());
			g_pImmediateContext->PSSetConstantBuffers(0, 1, m_Map->m_pConstantBuffer.GetAddressOf());
			g_pImmediateContext->VSSetConstantBuffers(1, 1, m_ConstantBuffer.GetAddressOf());
			g_pImmediateContext->VSSetShader(m_Map->m_pVertexShader.Get(), NULL, 0);
			g_pImmediateContext->PSSetShader(m_Map->m_pPixelShader.Get(), NULL, 0);
			g_pImmediateContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_Map->m_iTopology);
			if (m_Map->m_pTexture != nullptr)
			{
				g_pImmediateContext->PSSetShaderResources(0, 1,
					&m_Map->m_pTexture->m_pTextureSRV);
			}

			if (m_Map->m_pIndexBuffer == nullptr)
			{
				g_pImmediateContext->Draw(m_Map->m_VertexList.size(), 0);
			}
			else
			{
				g_pImmediateContext->DrawIndexed(m_Map->m_IndexList.size(), 0, 0);
			}




		if (BoolColl == true)
		{
			BoolColl = false;
			float Max = 99999;
			float f = 10.0f;
			Vector3 Pick;
			for (int i = 0; i < m_QuadTree.m_LeafNodeList.size(); i++)
			{
				if (Collision.SphereToRay(&m_QuadTree.m_LeafNodeList[i]->m_Sphere, m_Mouse.Orig, m_Mouse.Dir))
				{
					//m_SelectNode.push_back(m_QuadTree.m_LODDrawLIst[i]);
					float fDistance = (m_Mouse.Orig - Collision.m_vIntersection).Length();
					if (fDistance < Max)
					{
						Pick = Collision.m_vIntersection;
						Max = fDistance;
						BoolColl = true;
					}
				}
			}
			if (BoolColl)
			{
				Matrix mat;
				mat._41 = Pick.x;
				mat._42 = Pick.y;
				mat._43 = Pick.z;
				m_MatrixList.push_back(mat);
				/*for (int i = 0; i < m_Map->m_VertexList.size(); i++)
				{
					float fDist = (m_Map->m_VertexList[i].p - Pick).Length();
					if (fDist < f)
					{
						m_Map->m_VertexList[i].p.y = m_Map->m_VertexList[i].p.y + 5.0f - sinf((fDist / f));
					}
				}*/
			}
			//m_Map->UpdateVertexBuffer(g_pImmediateContext, &m_Map->m_VertexList.at(0), 0);
			BoolColl = false;
		}


	}
	return true;
}
bool Sample::Release()
{
	if(m_Map != nullptr)m_Map->Release();
	m_pObj.Release();
	m_QuadTree.Release();
	return true;
}

bool Sample::Build(int tel, int cel, int ces, wstring tex)
{
	if (m_Map != nullptr) m_Map->Release();
	m_Map = nullptr;
	SMapDesc desc;
	//desc.iNumCols = sqrt(tel * cel) * sqrt(tel) + 1;// m_Map.m_iNumCols;
	//desc.iNumRows = sqrt(tel * cel) *sqrt(tel) + 1;// m_Map.m_iNumRows;
	desc.iNumCols = tel * cel + 1;// m_Map.m_iNumCols;
	desc.iNumRows = tel * cel + 1;// m_Map.m_iNumRows;
	desc.fCellDistance = ces;
	desc.fScaleHeight = 1.0f;
	desc.szTexFile = tex;
	desc.szPS = L"map.hlsl";
	desc.szVS = L"map.hlsl";
	m_Map = new SMap;
 	m_Map->CreateMap(g_pd3dDevice, g_pImmediateContext, desc);
	/*RC R;
	R.r.x = tel;
	R.r.y = cel;
	R.r.z = ces;
	R.r.w = sqrt(tel * cel);
	m_Map->m_RC.resize(1);
	m_Map->m_RC.push_back(R);
	ID3D11Buffer* vbrc =
		TBASIS_CORE_LIB::CreateVertexBuffer(TBASIS_CORE_LIB::g_pd3dDevice,
			&m_Map->m_RC.at(0),
			m_Map->m_RC.size(),
			sizeof(RC));
	m_Map->m_pRC.Attach(vbrc);*/
	m_QuadTree.GetUpdata(m_pMainCamera);

	int a = sqrt(tel);
	int b = 1;
	while (2 < a)
	{
		b++;
		a /= 2;
	}
	if (tel == 1) b = 0;
	m_QuadTree.GetUpdata(m_pMainCamera);
	m_QuadTree.m_MaxDepth = b;
	m_QuadTree.Build(m_Map);


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(SRCData);
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, NULL, m_ConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_MAPPED_SUBRESOURCE mr;
	hr = g_pImmediateContext->Map(m_ConstantBuffer.Get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &mr);
	if (SUCCEEDED(hr))
	{
		SRCData* pData = (SRCData*)mr.pData;
		pData->vRc[0] = tel;
		pData->vRc[1] = cel;
		pData->vRc[2] = ces;
		pData->vRc[3] = tel * cel;

		g_pImmediateContext->Unmap(m_ConstantBuffer.Get(), 0);
	}

	return true;
}

