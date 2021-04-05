#include "main.h"
bool main::Init()
{
	m_Camera.CreateViewMatrix(
		{ 0,20,-50 },
		{ 0,0, 0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_Camera.CreateProjMatrix(1, 5000, TBASIS_PI / 4.0f, fAspect);

	m_Obj = make_shared<SFbxObj>();
	//if (m_Obj->Load("../../data/3DS/man.fbx"))
	//if (m_Obj->Load("../../data/3DS/Turret_Deploy1.fbx"))
	if (m_Obj->Load("../../data/3DS/Scifi_Model_L2_all_in_one.fbx"))
	{
		for (auto data : m_Obj->m_sNodeList)
		{
			SModelObj* pObject = data;
			for (int iSub = 0; iSub < pObject->subMesh.size(); iSub++)
			{
				SSubMesh* pSub = &pObject->subMesh[iSub];
				if (pSub->m_iNumFace <= 0) continue;

				ID3D11Buffer* vb =
					CreateVertexBuffer(g_pd3dDevice,
						&pSub->m_VertexList.at(0),
						pSub->m_VertexList.size(),
						sizeof(PNCT_VERTEX));
				pSub->m_pVertexBuffer.Attach(vb);

				ID3D11Buffer* vbiw =
					CreateVertexBuffer(g_pd3dDevice,
						&pSub->m_VertexListIW.at(0),
						pSub->m_VertexListIW.size(),
						sizeof(IW_VERTEX));
				pSub->m_pVertexBufferIW.Attach(vbiw);

				ID3D11Buffer* ib =
					CreateIndexBuffer(g_pd3dDevice,
						&pSub->m_IndexArray.at(0),
						pSub->m_IndexArray.size(),
						sizeof(DWORD));
				pSub->m_pIndexBuffer.Attach(ib);

				wstring loadTex = L"../../data/3DS/";
				loadTex += pObject->fbxMaterialList[iSub].c_str();
				pSub->m_pTexture = g_TextMgr.Load(g_pd3dDevice, loadTex.c_str());
			}
		}
		if (!m_Obj->Create(TBASIS_CORE_LIB::g_pd3dDevice,
			L"vs.txt",
			L"ps.txt",
			L""))
		{
			return false;
		}
	}


	D3D11_BUFFER_DESC vbdesc =
	{
		MAX_BONE_MATRICES * sizeof(Matrix),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, //D3D11_BIND_SHADER_RESOURCE,
		D3D11_CPU_ACCESS_WRITE,
		0
	};
	m_pd3dDevice->CreateBuffer(&vbdesc, NULL, m_Obj->m_BoneBuffer.GetAddressOf());

	return true;
}
bool main::Frame()
{
	m_Obj->m_fTick += g_fSecondPerFrame *
		m_Obj->m_Scene.iFrameSpeed *
		m_Obj->m_Scene.iTickPerFrame;// *0. 0f;

	if (m_Obj->m_fTick >=
		(m_Obj->m_Scene.iLastFrame *
			m_Obj->m_Scene.iTickPerFrame))
	{
		m_Obj->m_fTick = 0;
	}
	for (int iNode = 0; iNode < m_Obj->m_sNodeList.size(); iNode++)
	{
		Matrix matWorld = Matrix::Identity;
		SModelObj* pModelObject = m_Obj->m_sNodeList[iNode];
		std::string szName;
		szName.assign(pModelObject->m_szName.begin(), pModelObject->m_szName.end());
		Matrix matBiped = Matrix::Identity;
		auto data = m_Obj->m_dxMatrixBindPoseMap.find(szName);
		if (data != m_Obj->m_dxMatrixBindPoseMap.end())
		{
			matBiped = data->second;
		}

		for (int iTick = 0; iTick < pModelObject->animlist.size(); iTick++)
		{
			if (pModelObject->animlist[iTick].iTick >=
				m_Obj->m_fTick)
			{
				matWorld = pModelObject->animlist[iTick].mat;
				m_Obj->m_pMatrixList[iNode] = matBiped * matWorld;
				break;
			}
		}
	}
	Matrix* pMatrices;
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(g_pImmediateContext->Map((ID3D11Resource*)m_Obj->m_BoneBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest)))
	{
		pMatrices = (Matrix*)MappedFaceDest.pData;
		for (int dwObject = 0; dwObject < m_Obj->m_sNodeList.size(); dwObject++)
		{
			Matrix matAnim = m_Obj->m_pMatrixList[dwObject].Transpose();
			pMatrices[dwObject] = matAnim;
		}
		g_pImmediateContext->Unmap(m_Obj->m_BoneBuffer.Get(), 0);
	}
	return true;
}
bool main::Render()
{
	g_pImmediateContext->VSSetConstantBuffers(1, 1, m_Obj->m_BoneBuffer.GetAddressOf());

	m_Obj->SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);

	m_Obj->m_cbData.vColor[0] = m_pMainCamera->m_vLook.x;
	m_Obj->m_cbData.vColor[1] = m_pMainCamera->m_vLook.y;
	m_Obj->m_cbData.vColor[2] = m_pMainCamera->m_vLook.z;
	m_Obj->SDxObject::Update(g_pImmediateContext);
	g_pImmediateContext->IASetInputLayout(m_Obj->m_pInputLayout.Get());
	g_pImmediateContext->VSSetConstantBuffers(0, 1, m_Obj->m_pConstantBuffer.GetAddressOf());
	g_pImmediateContext->PSSetConstantBuffers(0, 1, m_Obj->m_pConstantBuffer.GetAddressOf());
	g_pImmediateContext->VSSetShader(m_Obj->m_pVertexShader.Get(), NULL, 0);
	g_pImmediateContext->PSSetShader(m_Obj->m_pPixelShader.Get(), NULL, 0);
	g_pImmediateContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_Obj->m_iTopology);


	for (int iNode = 0; iNode < m_Obj->m_sNodeList.size(); iNode++)
	{
		Matrix matWorld = Matrix::Identity;
		SModelObj* pObject = m_Obj->m_sNodeList[iNode];

		for (int iSub = 0; iSub < pObject->subMesh.size(); iSub++)
		{
			SSubMesh* pMesh = &pObject->subMesh[iSub];
			if (pMesh->m_iNumFace <= 0) continue;

			ID3D11Buffer* vb[2] = { pMesh->m_pVertexBuffer.Get(),	pMesh->m_pVertexBufferIW.Get() };
			UINT stride[2] = { sizeof(PNCT_VERTEX), sizeof(IW_VERTEX) };
			UINT offset[2] = { 0, 0 };
			g_pImmediateContext->IASetVertexBuffers(0, 2, vb, stride, offset);
			g_pImmediateContext->IASetIndexBuffer(pMesh->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			if (pMesh->m_pTexture != nullptr)
			{
				g_pImmediateContext->PSSetShaderResources(0, 1, &pMesh->m_pTexture->m_pTextureSRV);
			}
			else
			{
				g_pImmediateContext->PSSetShaderResources(0, 1, &g_TextMgr.m_pWhilteTexture->m_pTextureSRV);
			}
			if (pMesh->m_pIndexBuffer == nullptr)
			{
				g_pImmediateContext->Draw(pMesh->m_VertexList.size(), 0);
			}
			else
			{
				g_pImmediateContext->DrawIndexed(pMesh->m_IndexArray.size(), 0, 0);
			}
		}
	}
	return true;
}
bool main::Release()
{
		for (auto data : m_Obj->m_sNodeList)
		{
			SModelObj* pObject = data;
			pObject->Release();
			delete pObject;
		}
	return true;
}