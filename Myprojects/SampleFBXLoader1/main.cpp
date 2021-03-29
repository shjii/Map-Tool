#include "main.h"
bool main::Init()
{
	/*m_fbxObj.Load("../../data/3DS/rockBlobFive.FBX");*/
	//m_fbxObj.Load("../../data/3DS/BoxtoBox.FBX");

	const char* fbxobject[] =
	{
		"../../data/3DS/man.fbx",
		//"../../data/3DS/Turret_Deploy1.fbx",
		//"../../data/3DS/rockBlobFive.fbx",
		//"../../data/3DS/rockBlobFour.fbx",
		//"../../data/3DS/ship.fbx",
		//"../../data/3DS/SM_Rock.fbx",
		//"../../data/3DS/SM_House_Var01.fbx",
	};

	for (int i = 0; i < ARRAYSIZE(fbxobject); i++)
	{
		int c = ARRAYSIZE(fbxobject);
		shared_ptr<SFbxObj> obj = make_shared<SFbxObj>();

		if (!obj->Load(fbxobject[i])) continue;

		for (auto data : obj->m_sMeshList)
		{
			SModelObject* pObject = data;

			if (pObject->m_TriangleList.size() <= 0 &&
				pObject->m_subMesh.size() <= 0)
			{
				continue;
			}

			if (pObject->m_subMesh.size() == 0)
			{
				//if (pObject->m_TriangleList.empty()) continue;
				pObject->m_VertexList.resize(pObject->m_TriangleList.size() * 3);
				for (int iFace = 0; iFace < pObject->m_TriangleList.size(); iFace++)
				{
					int iIndex = iFace * 3;
					pObject->m_VertexList[iIndex + 0] = pObject->m_TriangleList[iFace].vVertex[0];
					pObject->m_VertexList[iIndex + 1] = pObject->m_TriangleList[iFace].vVertex[1];
					pObject->m_VertexList[iIndex + 2] = pObject->m_TriangleList[iFace].vVertex[2];
				}
				T_STR loadTexName = L"../../data/3DS/";
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

					if (pObject->m_subMesh[i].m_TriangleList.empty()) continue;

					pObject->m_subMesh[i].m_VertexList.resize(pObject->m_subMesh[i].m_TriangleList.size() * 3);
					for (int iFace = 0; iFace < pObject->m_subMesh[i].m_TriangleList.size(); iFace++)
					{
						int iIndex = iFace * 3;
						pObject->m_subMesh[i].m_VertexList[iIndex + 0] = pObject->m_subMesh[i].m_TriangleList[iFace].vVertex[0];
						pObject->m_subMesh[i].m_VertexList[iIndex + 1] = pObject->m_subMesh[i].m_TriangleList[iFace].vVertex[1];
						pObject->m_subMesh[i].m_VertexList[iIndex + 2] = pObject->m_subMesh[i].m_TriangleList[iFace].vVertex[2];
					}
					ID3D11Buffer* vb = CreateVertexBuffer(g_pd3dDevice, &pObject->m_subMesh[i].m_VertexList.at(0),
						pObject->m_subMesh[i].m_VertexList.size(), sizeof(PNCT_VERTEX));
					pObject->m_subMesh[i].m_pVertexBuffer.Attach(vb);

					wstring Tex = L"../../data/3DS/";
					Tex += pObject->fbxMaterialList[i].c_str();
					pObject->m_subMesh[i].m_pTexture = g_TextMgr.Load(g_pd3dDevice, Tex.c_str());
					if (!pObject->Create(TBASIS_CORE_LIB::g_pd3dDevice, L"vs.txt", L"ps.txt", L""))
					{
						return false;
					}
				}
			}
		}
		ObjList.push_back(obj);
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
	for (int i = 0; i < ObjList.size(); i++)
	{
		ObjList[i]->m_fTick += g_fSecondPerFrame *
			ObjList[i]->m_Scene.iFrameSpeed *
			ObjList[i]->m_Scene.iTickPerFrame;// *0.1f;

		if (ObjList[i]->m_fTick >=
			(ObjList[i]->m_Scene.iLastFrame *
				ObjList[i]->m_Scene.iTickPerFrame))
		{
			ObjList[i]->m_fTick = 0;
		}
		for (auto data : ObjList[i]->m_sMeshList)
		{
			Matrix matWorld = Matrix::Identity;
			SModelObject* pObject = data;
			for (int iTick = 0; iTick < pObject->animlist.size(); iTick++)
			{
				if (pObject->animlist.size() == 1)
				{
					matWorld = pObject->animlist[iTick].mat;
					break;
				}
				else if (pObject->animlist[iTick].iTick >= ObjList[i]->m_fTick)
				{
					matWorld = pObject->animlist[iTick].mat;
					break;
				}
			}
			
			if (pObject->m_subMesh.size() <= 0)
			{
				if (pObject->m_TriangleList.empty()) continue;
				pObject->m_cbData.vColor[0] = m_pMainCamera->m_vLook.x;
				pObject->m_cbData.vColor[1] = m_pMainCamera->m_vLook.y;
				pObject->m_cbData.vColor[2] = m_pMainCamera->m_vLook.z;
				pObject->SetMatrix(&pObject->m_matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
				pObject->Render(g_pImmediateContext);
			}
			else
			{
				for (int i = 0; i < pObject->m_subMesh.size(); i++)
				{
					pObject->m_cbData.vColor[0] = m_pMainCamera->m_vLook.x;
					pObject->m_cbData.vColor[1] = m_pMainCamera->m_vLook.y;
					pObject->m_cbData.vColor[2] = m_pMainCamera->m_vLook.z;
					pObject->SetMatrix(&matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
					pObject->m_VertexList = pObject->m_subMesh[i].m_VertexList;
					//	pObject->m_pVertexBuffer = pObject->m_subMesh[i].m_pVertexBuffer.Get();
					pObject->m_pTexture = pObject->m_subMesh[i].m_pTexture;
					pObject->PreRender(g_pImmediateContext);
					UINT iStride = sizeof(PNCT_VERTEX);
					UINT iOffset = 0;
					g_pImmediateContext->IASetVertexBuffers(0, 1, pObject->m_subMesh[i].m_pVertexBuffer.GetAddressOf(), &iStride, &iOffset);
					pObject->PostRender(g_pImmediateContext);
				}
			}
		}
	}
	return true;
}
bool main::Release()
{
	for (int i = 0; i < ObjList.size(); i++)
	{
		for (auto data : ObjList[i]->m_sMeshList)
		{
			SModelObject* pObject = (SModelObject*)data;
			pObject->Release();
			delete pObject;
		}
	}
	ObjList.clear();
	return true;
}