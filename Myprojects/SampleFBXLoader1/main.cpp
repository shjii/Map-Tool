#include "main.h"
bool main::Init()
{
	m_fbxObj.Load("../../data/3DS/ship.FBX");
	//m_fbxObj.Load("../../data/3DS/BoxtoBox.FBX");
	for (auto data : m_fbxObj.m_sMeshMap)
	{
		SModelObject* pObject = (SModelObject*)data.second;
		if (pObject->m_TriangleList.empty() && pObject->m_subMesh.size() <= 0) continue;
		if (pObject->m_subMesh.size() <= 0)
		{
			pObject->m_VertexList.resize(pObject->m_TriangleList.size() * 3);
			for (int iFace = 0; iFace < pObject->m_TriangleList.size(); iFace++)
			{
				int iIndex = iFace * 3;
				pObject->m_VertexList[iIndex + 0] = pObject->m_TriangleList[iFace].vVertex[0];
				pObject->m_VertexList[iIndex + 1] = pObject->m_TriangleList[iFace].vVertex[1];
				pObject->m_VertexList[iIndex + 2] = pObject->m_TriangleList[iFace].vVertex[2];
			}
			T_STR loadTexName = L"../../data/bitmap/";
			loadTexName += pObject->fbxMaterialList[0];
			if (!pObject->Create(TBASIS_CORE_LIB::g_pd3dDevice, L"vs.txt", L"ps.txt", loadTexName))
			{
				return false;
			}
		}
		else
		{
			for (int i = 0; i < pObject->m_subMesh.size(); i++)
			{
				pObject->m_subMesh[i].m_VertexList.resize(pObject->m_subMesh[i].m_TriangleList.size() * 3);
				for (int iFace = 0; iFace < pObject->m_subMesh[i].m_TriangleList.size(); iFace++)
				{
					int iIndex = iFace * 3;
					pObject->m_subMesh[i].m_VertexList[iIndex + 0] = pObject->m_subMesh[i].m_TriangleList[iFace].vVertex[0];
					pObject->m_subMesh[i].m_VertexList[iIndex + 1] = pObject->m_subMesh[i].m_TriangleList[iFace].vVertex[1];
					pObject->m_subMesh[i].m_VertexList[iIndex + 2] = pObject->m_subMesh[i].m_TriangleList[iFace].vVertex[2];
				}
				T_STR loadTexName = L"../../data/bitmap/";
				loadTexName += pObject->fbxMaterialList[0];
				ID3D11Buffer* vb = CreateVertexBuffer(g_pd3dDevice, &pObject->m_subMesh[i].m_VertexList.at(0),
														pObject->m_subMesh[i].m_VertexList.size(), sizeof(PNCT_VERTEX));
				pObject->m_subMesh[i].m_pVertexBuffer.Attach(vb);

				wstring Tex = L"../../data/3DS/";
				Tex += pObject->fbxMaterialList[i].c_str();
				pObject->m_subMesh[i].m_pTexture = g_TextMgr.Load(g_pd3dDevice, Tex.c_str());
				if (!pObject->Create(TBASIS_CORE_LIB::g_pd3dDevice, L"vs.txt", L"ps.txt",  L""))
				{
					return false;
				}
			}
		}
	}
	return true;
}
bool main::Frame()
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
	if (g_Input.GetKey('8') == KEY_PUSH)
	{
		SDxState::m_CullMode = D3D11_CULL_BACK;
		SDxState::SetRasterizerState(g_pd3dDevice);
	}
	if (g_Input.GetKey('7') == KEY_PUSH)
	{
		SDxState::m_CullMode = D3D11_CULL_FRONT;
		SDxState::SetRasterizerState(g_pd3dDevice);
	}
	return true;
}
bool main::Render()
{
	for (auto data : m_fbxObj.m_sMeshMap)
	{
		SModelObject* pObject = (SModelObject*)data.second;
		if (pObject->m_TriangleList.empty() && pObject->m_subMesh.size() <= 0) continue;
		pObject->SetMatrix(&pObject->m_matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		if (pObject->m_subMesh.size() <= 0)
		{
			pObject->Render(g_pImmediateContext);
		}
		else
		{
			for (int i = 0; i < pObject->m_subMesh.size(); i++)
			{
				pObject->m_VertexList = pObject->m_subMesh[i].m_VertexList;
				pObject->m_pVertexBuffer = pObject->m_subMesh[i].m_pVertexBuffer.Get();
				pObject->m_pTexture = pObject->m_subMesh[i].m_pTexture;
				pObject->Render(g_pImmediateContext);
			}
		}
	}
	return true;
}
bool main::Release()
{
	for (auto data : m_fbxObj.m_sMeshMap)
	{
		SModelObject* pObject = (SModelObject*)data.second;
		pObject->Release();
		delete pObject;
	}
	return true;
}