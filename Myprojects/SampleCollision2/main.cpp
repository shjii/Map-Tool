#include "main.h"

bool main::Init()
{
	HRESULT hr;
	m_pConstantBufferLight[0].Attach(TBASIS_CORE_LIB::CreateConstantBuffer(g_pd3dDevice, &m_cbLight1, 1, sizeof(LIGHT_CONSTANT_BUFFER1)));
	m_pConstantBufferLight[1].Attach(TBASIS_CORE_LIB::CreateConstantBuffer(g_pd3dDevice, &m_cbLight2, 1, sizeof(LIGHT_CONSTANT_BUFFER1)));
	m_pConstantBufferLight[2].Attach(TBASIS_CORE_LIB::CreateConstantBuffer(g_pd3dDevice, &m_cbLight3, 1, sizeof(LIGHT_CONSTANT_BUFFER1)));
	//m_Map.CreateHeightMap(g_pd3dDevice, g_pImmediateContext, L"../../data/map/heightMap513.bmp");

	m_pObj.Init();
	m_pObj.m_pMainCamera = m_pMainCamera;

	SMapDesc desc;
	desc.iNumCols = 513;// m_Map.m_iNumCols;
	desc.iNumRows = 513;// m_Map.m_iNumRows;
	desc.fCellDistance = 1;
	desc.fScaleHeight = 1.0f;
	desc.szTexFile = L"../../data/map/grasshill.jpg";
	desc.szPS = L"ps.txt";
	desc.szVS = L"vs.txt";

	//m_MinMap.Create(g_pd3dDevice, L"vs.txt", L"ps.txt",
	//	L"../../data/bitmap/tileA.jpg");

	m_pMainCamera->Create(g_pd3dDevice);
	m_pMainCamera->CreateViewMatrix({ 0,100,-10 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_pMainCamera->CreateProjMatrix(1, 50000, TBASIS_PI / 4.0f, fAspect);
	m_Map.CreateMap(g_pd3dDevice, g_pImmediateContext, desc);

	//m_Map.InitNormal();
	//m_Map.FindingNormal();

	m_QuadTree.GetUpdata(m_pMainCamera);
	m_QuadTree.m_MaxDepth = 5;
	m_QuadTree.Build(&m_Map);
	///
	//m_TopCamera.CreateViewMatrix({ 0,1000,-0.1f }, { 0,0,0 });
	//m_TopCamera.CreateOrthographic(
	//	desc.iNumCols, desc.iNumRows, 1.0f, 1000);
	//m_TopCamera.Init();
	return true;
}
bool main::Frame()
{
	m_cbLight1.g_cAmbientMaterial[0] = Vector4(0.1f, 0.1f, 0.1f, 1);
	m_cbLight1.g_cDiffuseMaterial[0] = Vector4(1, 1, 1, 1);
	m_cbLight1.g_cSpecularMaterial[0] = Vector4(1, 1, 1, 1);
	m_cbLight1.g_cEmissionMaterial[0] = Vector4(0, 0, 0, 1);
	m_cbLight1.g_cAmbientLightColor[0] = Vector4(0.3f, 0.3f, 0.3f, 1);
	m_cbLight1.g_cSpecularLightColor[0] = Vector4(1, 1, 1, 1);
	m_cbLight1.g_cDiffuseLightColor[0] = Vector4(1, 0, 0, 1.0f);
	m_cbLight1.g_cAmbientLightColor[1] = Vector4(0.3f, 0.3f, 0.3f, 1);
	m_cbLight1.g_cSpecularLightColor[1] = Vector4(1, 1, 1, 1);
	m_cbLight1.g_cDiffuseLightColor[1] = Vector4(0, 1, 0, 1.0f);
	m_cbLight1.g_cAmbientLightColor[2] = Vector4(0.3f, 0.3f, 0.3f, 1);
	m_cbLight1.g_cSpecularLightColor[2] = Vector4(1, 1, 1, 1);
	m_cbLight1.g_cDiffuseLightColor[2] = Vector4(0, 0, 1, 1.0f);

	float	fTime = 32.0f;
	m_cbLight2.g_vLightPos[0] = Vector4(0, 30, 0, 200);
	m_cbLight2.g_vLightPos[1] = Vector4(50, 0, 0, 200);
	m_cbLight2.g_vLightPos[2] = Vector4(0, 0, 50, 200);

	m_cbLight2.g_vLightDir[0] = -m_cbLight2.g_vLightPos[0];
	m_cbLight2.g_vLightDir[0].w = 1.0f;
	m_cbLight2.g_vLightDir[0].Normalize();
	m_cbLight2.g_vLightDir[1] = -m_cbLight2.g_vLightPos[1];
	m_cbLight2.g_vLightDir[1].w = 1.0f;
	m_cbLight2.g_vLightDir[1].Normalize();
	m_cbLight2.g_vLightDir[2] = -m_cbLight2.g_vLightPos[2];
	m_cbLight2.g_vLightDir[2].w = 1.0f;
	m_cbLight2.g_vLightDir[2].Normalize();

	for (int iLight = 0; iLight < g_iNumLight; iLight++)
	{
		Matrix	matInvWorld;
		m_cbLight2.g_matInvWorld[iLight] = Matrix::Identity;
		m_cbLight2.g_vEyeDir[iLight].x = m_pMainCamera->m_vLook.x;
		m_cbLight2.g_vEyeDir[iLight].y = m_pMainCamera->m_vLook.y;
		m_cbLight2.g_vEyeDir[iLight].z = m_pMainCamera->m_vLook.z;
		m_cbLight2.g_vEyeDir[iLight].w = 50.0f;
	}

	m_cbLight3.g_vSpotInfo[0].x = 20.0f;// 내부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[0].y = 30.0f;// 외부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[0].z = 4;	// 내부 콘과 외부 콘의 휘도( Luminance )	
	m_cbLight3.g_vSpotInfo[0].w = 100;// 범위	
	m_cbLight3.g_vSpotInfo[1].x = 20.0f;// 내부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[1].y = 30.0f;// 외부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[1].z = 4;	// 내부 콘과 외부 콘의 휘도( Luminance )	
	m_cbLight3.g_vSpotInfo[1].w = 100;// 범위	
	m_cbLight3.g_vSpotInfo[2].x = 20.0f;// 내                                                  부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[2].y = 30.0f;// 외부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[2].z = 4;	// 내부 콘과 외부 콘의 휘도( Luminance )	
	m_cbLight3.g_vSpotInfo[2].w = 100;// 범위	

	m_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[0].Get(), 0, NULL, &m_cbLight1, 0, 0);
	m_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[1].Get(), 0, NULL, &m_cbLight2, 0, 0);
	m_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[2].Get(), 0, NULL, &m_cbLight3, 0, 0);
	ID3D11Buffer*	pBuffers[3];
	pBuffers[0] = m_pConstantBufferLight[0].Get();
	pBuffers[1] = m_pConstantBufferLight[1].Get();
	pBuffers[2] = m_pConstantBufferLight[2].Get();
	m_pImmediateContext->PSSetConstantBuffers(1, 3, pBuffers);

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
	if (g_Input.GetKey(VK_RBUTTON) == KEY_HOLD)
	{
		m_Mouse.RayFrame(m_pMainCamera->m_matWorld, m_pMainCamera->m_matView, m_pMainCamera->m_matProj);
		BoolColl = true;
	}

	/*
	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_UserShape.FrontMovement(1.0f);
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_UserShape.FrontMovement(-1.0f);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_UserShape.RightMovement(-1.0f);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_UserShape.RightMovement(1.0f);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_UserShape.UpMovement(1.0f);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_UserShape.UpMovement(-1.0f);
	}*/ 
	
	m_QuadTree.Frame();

	//m_Map.UpdateIndexBuffer(g_pImmediateContext, &m_QuadTree.m_IndexList.at(0), m_QuadTree.Face);
	//m_Map.m_IndexList = m_QuadTree.m_IndexList;
	m_pObj.Frame();
	m_Map.Frame();
	m_pMainCamera->Frame();
	m_SelectNode.clear();
	return true;
}
bool main::Render()
{
	for (auto data : m_MatrixList)
	{
		m_pObj.m_Obj->SetMatrix(&data, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_pObj.Render(g_pImmediateContext);
	}
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->RSSetState(SDxState::m_pRS);
	g_pImmediateContext->PSSetSamplers(0, 1, &SDxState::m_pWrapLinear);
	g_pImmediateContext->OMSetDepthStencilState(SDxState::m_pDSS, 0);



	m_Map.SetMatrix(NULL,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);

	if (BoolColl == true)
	{
		BoolColl = false;
		float Max = 99999;
		float f = 10.0f;
		Vector3 Pick;
		for (int i = 0; i < m_QuadTree.m_LODDrawLIst.size(); i++)
		{
			//if (Collision.AABBToRay(&m_QuadTree.m_LODDrawLIst[i]->m_Box, m_Mouse.Orig, m_Mouse.Dir))
			//{
			//	BoolColl = true;
			//	m_SelectNode.push_back(m_QuadTree.m_LODDrawLIst[i]);
			//}

			if (Collision.SphereToRay(&m_QuadTree.m_LODDrawLIst[i]->m_Sphere, m_Mouse.Orig, m_Mouse.Dir))
			{
				m_SelectNode.push_back(m_QuadTree.m_LODDrawLIst[i]);
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
			for (int i = 0; i < m_Map.m_VertexList.size(); i++)
			{
				float fDist = (m_Map.m_VertexList[i].p - Pick).Length();
				if (fDist < f)
				{
					m_Map.m_VertexList[i].p.y = m_Map.m_VertexList[i].p.y + 5.0f - sinf((fDist / f));
				}
			}
		}
		m_Map.UpdateVertexBuffer(g_pImmediateContext, &m_Map.m_VertexList.at(0), 0);
		BoolColl = false;
	}
	m_Map.Render(g_pImmediateContext);
	m_LineShape.SetMatrix(NULL, &m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_LineShape.Draw(g_pImmediateContext, m_Mouse.Orig, m_Mouse.Orig + m_Mouse.Dir * 100.0f);

	return true;
}
bool main::PostRender()
{
	TCore::PostRender();
	return true;
}
void main::Draw(SNode *Node, ID3D11DeviceContext * pd3dContext)
{
	m_Map.PreRender(pd3dContext);
	pd3dContext->IASetIndexBuffer(Node->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pd3dContext->DrawIndexed(Node->m_IndexList.size(), 0, 0);
}
bool main::Release()
{
	m_pObj.Release();
	m_Map.Release();
	m_MinMap.Release();
	m_QuadTree.Release();
	return true;
}