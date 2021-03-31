#include "main.h"
bool main::Init()
{
	m_Obj = make_shared<SFbxObj>();

	if (m_Obj->Load("../../data/3DS/man.fbx"))
	//if (m_Obj->Load("../../data/3DS/Turret_Deploy1.fbx"))
	{
		for (auto data : m_Obj->m_sNodeList)
		{
			SModelObj* pObject = data;
			for (int i = 0; i < pObject->m_subMesh.size(); i++)
			{
				subMesh* pSub = &pObject->m_subMesh[i];
				if (pSub->m_TriangleList.empty()) continue;

				pSub->m_VertexList.resize(pSub->m_TriangleList.size() * 3);
				for (int iFace = 0; iFace < pSub->m_TriangleList.size(); iFace++)
				{
					int iIndex = iFace * 3;
					pSub->m_VertexList[iIndex + 0] = pSub->m_TriangleList[iFace].vVertex[0];
					pSub->m_VertexList[iIndex + 1] = pSub->m_TriangleList[iFace].vVertex[1];
					pSub->m_VertexList[iIndex + 2] = pSub->m_TriangleList[iFace].vVertex[2];
				}
				ID3D11Buffer* vb = CreateVertexBuffer(g_pd3dDevice, &pSub->m_VertexList.at(0),
					pSub->m_VertexList.size(), sizeof(PNCT_VERTEX));
				pSub->m_pVertexBuffer.Attach(vb);

				pSub->m_VertexListIW.resize(pSub->m_TriangleList.size() * 3);
				for (int iFace = 0; iFace < pSub->m_TriangleList.size(); iFace++)
				{
					int iIndex = iFace * 3;
					pSub->m_VertexListIW[iIndex + 0] = pSub->m_TriangleList[iFace].vVertexIW[0];
					pSub->m_VertexListIW[iIndex + 1] = pSub->m_TriangleList[iFace].vVertexIW[1];
					pSub->m_VertexListIW[iIndex + 2] = pSub->m_TriangleList[iFace].vVertexIW[2];
				}
				ID3D11Buffer* vbiw = CreateVertexBuffer(g_pd3dDevice, &pSub->m_VertexListIW.at(0),
					pSub->m_VertexListIW.size(), sizeof(IW_VERTEX));
				pSub->m_pVertexBufferIW.Attach(vbiw);

				wstring Tex = L"../../data/3DS/";
				Tex += pObject->fbxMaterialList[i].c_str();
				pSub->m_pTexture = g_TextMgr.Load(g_pd3dDevice, Tex.c_str());

			}
			if (!pObject->Create(TBASIS_CORE_LIB::g_pd3dDevice, L"vs.txt", L"ps.txt", L""))
			{
				return false;
			}
		}
	}
	D3D11_BUFFER_DESC vbdesc =
	{
		MAX_BONE_MATRICES * sizeof(Matrix),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
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
		m_Obj->m_Scene.iTickPerFrame *0.0f;

	if (m_Obj->m_fTick >=
		(m_Obj->m_Scene.iLastFrame *
			m_Obj->m_Scene.iTickPerFrame))
	{
		m_Obj->m_fTick = 0;
	}
	for (int iNode = 0; iNode < m_Obj->m_sNodeMap.size(); iNode++)
	{
		Matrix matWorld = Matrix::Identity;
		SModelObj* pObject = m_Obj->m_sNodeList[iNode];
		for (int iTick = 0; iTick < pObject->animlist.size(); iTick++)
		{
			if (pObject->animlist[iTick].iTick >= m_Obj->m_fTick)
			{
				string szName;
				szName.assign(pObject->m_szName.begin(), pObject->m_szName.end());
				Matrix matBiped = Matrix::Identity;

				auto data = m_Obj->m_dxMatrixMap.find(szName);
				if (data != m_Obj->m_dxMatrixMap.end())
				{
					matBiped = data->second;
				}
				matWorld = pObject->animlist[iTick].mat;
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
	g_pImmediateContext->VSSetConstantBuffers(1,1, m_Obj->m_BoneBuffer.GetAddressOf());

	for (int iNode = 0; iNode < m_Obj->m_sNodeList.size(); iNode++)
	{
		Matrix matWorld = Matrix::Identity;
		SModelObj* pObject = m_Obj->m_sNodeList[iNode];
		pObject->SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);

		pObject->m_cbData.vColor[0] = m_pMainCamera->m_vLook.x;
		pObject->m_cbData.vColor[1] = m_pMainCamera->m_vLook.y;
		pObject->m_cbData.vColor[2] = m_pMainCamera->m_vLook.z;
		pObject->SDxObject::Update(g_pImmediateContext);
		g_pImmediateContext->IASetIndexBuffer(pObject->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		g_pImmediateContext->IASetInputLayout(pObject->m_pInputLayout);
		g_pImmediateContext->VSSetConstantBuffers(0, 1, &pObject->m_pConstantBuffer);
		g_pImmediateContext->PSSetConstantBuffers(0, 1, &pObject->m_pConstantBuffer);
		g_pImmediateContext->VSSetShader(pObject->m_pVertexShader, NULL, 0);
		g_pImmediateContext->PSSetShader(pObject->m_pPixelShader, NULL, 0);
		g_pImmediateContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)pObject->m_iTopology);

		for (int i = 0; i < pObject->m_subMesh.size(); i++)
		{
			subMesh* pMesh = &pObject->m_subMesh[i];
			if (pMesh->m_TriangleList.empty()) continue;



			ID3D11Buffer* vb[2] = { pMesh->m_pVertexBuffer.Get(), pMesh->m_pVertexBufferIW.Get() };
			UINT iStride[2] = { sizeof(PNCT_VERTEX) , sizeof(IW_VERTEX)};
			UINT iOffset[2] = { 0 , 0};
			g_pImmediateContext->IASetVertexBuffers(0, 2, vb, iStride, iOffset);
			if (pMesh->m_pTexture != nullptr)
			{
				g_pImmediateContext->PSSetShaderResources(0, 1, &pMesh->m_pTexture->m_pTextureSRV);
			}
			if (pObject->m_pIndexBuffer == nullptr)
			{
				g_pImmediateContext->Draw(pMesh->m_VertexList.size(), 0);
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