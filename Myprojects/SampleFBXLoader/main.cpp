#include "main.h"

bool main::Init()
{
	m_fbxObj.Load("../../data/bitmap/Card.FBX");

	for (auto data : m_fbxObj.m_sMeshMap)
	{
		TObject* pObject = (TObject*)data.second;
		if (pObject->m_TriangleList.empty()) continue;

		pObject->m_VertexList.resize(pObject->m_TriangleList.size() * 3);
		for (int iFace = 0; iFace < pObject->m_TriangleList.size(); iFace++)
		{
			int iIndex = iFace * 3;
			pObject->m_VertexList[iIndex + 0] = pObject->m_TriangleList[iFace].vVertex[0];
			pObject->m_VertexList[iIndex + 1] = pObject->m_TriangleList[iFace].vVertex[1];
			pObject->m_VertexList[iIndex + 2] = pObject->m_TriangleList[iFace].vVertex[2];
		}
		if (!pObject->Create(TBASIS_CORE_LIB::g_pd3dDevice, L"vs.txt", L"ps.txt", L"../../data/map/HEIGHT_CASTLE.bmp"));
		{
			return false;
		}
	}
	return true;
}
bool main::Frame()
{
	return true;
}
bool main::Render()
{
	for (auto data : m_fbxObj.m_sMeshMap)
	{
		TObject* pObject = (TObject*)data.second;
		if (pObject->m_TriangleList.size() <= 0) continue;

		pObject->SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		pObject->Render(g_pImmediateContext);
	}
	return true;
}
bool main::Release()
{
	return true;
}