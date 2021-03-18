#include "main.h"

bool main::Init()
{
	m_fbxObj.Load("../../data/3DS/BoxtoBox.FBX");

	for (auto data : m_fbxObj.m_sMeshMap)
	{
		SModelObject* pObject = (SModelObject*)data.second;
		if (pObject->m_TriangleList.empty()) continue;

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
	return true;
}
bool main::Frame()
{
	//if (g_Input.GetKey('0') == KEY_PUSH)
	//{
	//	SDxState::m_FillMode = D3D11_FILL_WIREFRAME;
	//	SDxState::SetRasterizerState(g_pd3dDevice);
	//}
	//if (g_Input.GetKey('9') == KEY_PUSH)
	//{
	//	SDxState::m_FillMode = D3D11_FILL_SOLID;
	//	SDxState::SetRasterizerState(g_pd3dDevice);
	//}
	//if (g_Input.GetKey('8') == KEY_PUSH)
	//{
	//	SDxState::m_CullMode = D3D11_CULL_BACK;
	//	SDxState::SetRasterizerState(g_pd3dDevice);
	//}
	//if (g_Input.GetKey('7') == KEY_PUSH)
	//{
	//	SDxState::m_CullMode = D3D11_CULL_FRONT;
	//	SDxState::SetRasterizerState(g_pd3dDevice);
	//}
	return true;
}
bool main::Render()
{
	for (auto data : m_fbxObj.m_sMeshMap)
	{
		TObject* pObject = (TObject*)data.second;
		if (pObject->m_TriangleList.size() <= 0) continue;

		pObject->SetMatrix(&pObject->m_matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		pObject->Render(g_pImmediateContext);
	}
	return true;
}
bool main::Release()
{
	return true;
}