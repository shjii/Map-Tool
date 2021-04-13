#include "main.h"
Matrix* TD3DXMatrixShadow(Matrix *pout,
	Vector4 *plight,
	Vector4 *pplane)
{
	Vector4 Nplane;
	FLOAT dot;
	Nplane.Normalize();
	//D3DXPlaneNormalize(&Nplane, pplane);
	dot = Nplane.Dot(*plight);
	pout->m[0][0] = dot - Nplane.x * plight->x;
	pout->m[0][1] = -Nplane.x * plight->y;
	pout->m[0][2] = -Nplane.x * plight->z;
	pout->m[0][3] = -Nplane.x * plight->w;
	pout->m[1][0] = -Nplane.y * plight->x;
	pout->m[1][1] = dot - Nplane.y * plight->y;
	pout->m[1][2] = -Nplane.y * plight->z;
	pout->m[1][3] = -Nplane.y * plight->w;
	pout->m[2][0] = -Nplane.z * plight->x;
	pout->m[2][1] = -Nplane.z * plight->y;
	pout->m[2][2] = dot - Nplane.z * plight->z;
	pout->m[2][3] = -Nplane.z * plight->w;
	pout->m[3][0] = -Nplane.w * plight->x;
	pout->m[3][1] = -Nplane.w * plight->y;
	pout->m[3][2] = -Nplane.w * plight->z;
	pout->m[3][3] = dot - Nplane.w * plight->w;
	return pout;
}
Matrix main::CreateMatrixShadow(
	Vector4* pPlane,
	Vector4* pLight)
{
	Matrix mat;
	Vector4 plane, light;
	pPlane->Normalize();
	plane.x = pPlane->x * -1.0f;
	plane.y = pPlane->y * -1.0f;
	plane.z = pPlane->z * -1.0f;
	plane.w = pPlane->w * -1.0f;
	light = *pLight;// * -1.0f;
	float D = -(plane.Dot(light));
	mat._11 = plane.x * light.x + D;	mat._12 = plane.x * light.y;	mat._13 = plane.x * light.z;	mat._14 = plane.x * light.w;
	mat._21 = plane.y * light.x;	mat._22 = plane.y * light.y + D;	mat._23 = plane.y * light.z;	mat._24 = plane.y * light.w;
	mat._31 = plane.z * light.x;	mat._32 = plane.z * light.y;	mat._33 = plane.z * light.z + D;	mat._34 = plane.z * light.w;
	mat._41 = plane.w * light.x;	mat._42 = plane.w * light.y;	mat._43 = plane.w * light.z;	mat._44 = plane.w * light.w + D;
	return mat;
}
LRESULT	 main::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera == nullptr) return -1;
	m_pMainCamera->WndProc(hWnd, message, wParam, lParam);
	return -1;
}
bool main::Init()
{
	HRESULT hr;

	m_pConstantBufferLight[0].Attach(TBASIS_CORE_LIB::CreateConstantBuffer(g_pd3dDevice, &m_cbLight1, 1, sizeof(LIGHT_CONSTANT_BUFFER1)));
	m_pConstantBufferLight[1].Attach(TBASIS_CORE_LIB::CreateConstantBuffer(g_pd3dDevice, &m_cbLight2, 1, sizeof(LIGHT_CONSTANT_BUFFER1)));
	m_pConstantBufferLight[2].Attach(TBASIS_CORE_LIB::CreateConstantBuffer(g_pd3dDevice, &m_cbLight3, 1, sizeof(LIGHT_CONSTANT_BUFFER1)));
	m_Map.CreateHeightMap(g_pd3dDevice, g_pImmediateContext, L"../../data/map/HEIGHT_CASTLE.bmp");

	SMapDesc desc;
	desc.iNumCols = m_Map.m_iNumCols;
	desc.iNumRows = m_Map.m_iNumRows;
	desc.fCellDistance = 1;
	desc.fScaleHeight = 10.0f;
	desc.szTexFile = L"../../data/map/castle.jpg";
	desc.szPS = L"ps.txt";
	desc.szVS = L"vs.txt";

	m_Map.CreateMap(g_pd3dDevice, g_pImmediateContext, desc);

	m_MinMap.Create(g_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/tileA.jpg");

	m_vDirValue = { 0,0,0,0 };

	Matrix matScale, matRotation;
	matScale = Matrix::CreateScale(1000, 1000, 1000);
	matRotation = Matrix::CreateRotationX(TBASIS_PI*0.5f);
	m_matPlaneWorld = matScale * matRotation;

	if (!m_BoxShape.Create(g_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/1KGCABK.bmp"))
	{
		return false;
	}
	if (!m_PlaneShape.Create(g_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/tileA.jpg"))
	{
		return false;
	}


	m_ModelCamera.CreateViewMatrix({ 0,10,-10 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_ModelCamera.CreateProjMatrix(1, 1000, TBASIS_PI / 4.0f, fAspect);
	m_ModelCamera.Init();
	m_ModelCamera.CrateFrustum(g_pd3dDevice, g_pImmediateContext);
	m_pMainCamera = &m_ModelCamera;

	m_TopCamera.CreateViewMatrix({ 0,30,-0.1f }, { 0,0,0 });
	fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_TopCamera.CreateOrthographic(
		desc.iNumCols, desc.iNumRows, 1.0f, 1000);
	m_TopCamera.Init();
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
	}

	m_cbLight3.g_vSpotInfo[0].x = cosf(30.0f*(3.141592f / 180.0f));// 내부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[0].y = cosf(30.0f*(3.141592f / 180.0f));;// 외부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[0].z = 4;	// 내부 콘과 외부 콘의 휘도( Luminance )	
	m_cbLight3.g_vSpotInfo[0].w = 100;// 범위	
	m_cbLight3.g_vSpotInfo[1].x = cosf(90.0f*(3.141592f / 180.0f));// 내부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[1].y = cosf(120.0f*(3.141592f / 180.0f));;// 외부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[1].z = 4;	// 내부 콘과 외부 콘의 휘도( Luminance )	
	m_cbLight3.g_vSpotInfo[1].w = 100;// 범위	
	m_cbLight3.g_vSpotInfo[2].x = cosf(90.0f*(3.141592f / 180.0f));// 내부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[2].y = cosf(120.0f*(3.141592f / 180.0f));;// 외부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[2].z = 4;	// 내부 콘과 외부 콘의 휘도( Luminance )	
	m_cbLight3.g_vSpotInfo[2].w = 100;// 범위

	m_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[0].Get(), 0, NULL, &m_cbLight1,0, 0);
	m_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[1].Get(), 0, NULL, &m_cbLight2, 0, 0);
	m_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[2].Get(), 0, NULL, &m_cbLight3, 0, 0);
	ID3D11Buffer*	pBuffers[3];
	pBuffers[0] = m_pConstantBufferLight[0].Get();
	pBuffers[1] = m_pConstantBufferLight[1].Get();
	pBuffers[2] = m_pConstantBufferLight[2].Get();
	m_pImmediateContext->PSSetConstantBuffers(1,3,pBuffers);
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

	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_BoxShape.FrontMovement(1.0f);
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_BoxShape.FrontMovement(-1.0f);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_BoxShape.RightMovement(-1.0f);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_BoxShape.RightMovement(1.0f);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_BoxShape.UpMovement(1.0f);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_BoxShape.UpMovement(-1.0f);
	}
	m_BoxShape.Frame();

	m_BoxShape.m_vPos.y = m_Map.GetHeightMap(m_BoxShape.m_matWorld._41, m_BoxShape.m_matWorld._43);


	m_pMainCamera->m_vCameraTarget = m_BoxShape.m_vPos;

	m_pMainCamera->FrameFrustum(g_pImmediateContext);
	m_BoxShape.m_matRotation = m_pMainCamera->m_matWorld;
	return true;
}
bool main::Render()
{
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->RSSetState(SDxState::m_pRS);
	g_pImmediateContext->PSSetSamplers(0, 1, &SDxState::m_pWrapLinear);
	g_pImmediateContext->OMSetDepthStencilState(SDxState::m_pDSS, 0);

	// CULLING
	//vector<DWORD> vislbeIB;
	//vislbeIB.resize(m_Map.m_IndexList.size());
	//m_Map.m_iNumFaces = 0;
	//for (int iFace = 0; iFace < m_Map.m_IndexList.size() / 3; iFace++)
	//{
	//	int a = m_Map.m_IndexList[iFace * 3 + 0];
	//	int b = m_Map.m_IndexList[iFace * 3 + 1];
	//	int c = m_Map.m_IndexList[iFace * 3 + 2];
	//	Vector3 v[3];
	//	v[0] = m_Map.m_VertexList[a].p;
	//	v[1] = m_Map.m_VertexList[b].p;
	//	v[2] = m_Map.m_VertexList[c].p;
	//	SModelViewCamera* pCamera = (SModelViewCamera*)m_pMainCamera;
	//	for (int iV = 0; iV < 3; iV++)
	//	{
	//		BOOL bVisiable = pCamera->m_Frustum.ClassifyPoint(v[iV]);
	//		if (bVisiable)
	//		{
	//			vislbeIB[m_Map.m_iNumFaces * 3 + 0] = a;
	//			vislbeIB[m_Map.m_iNumFaces * 3 + 1] = b;
	//			vislbeIB[m_Map.m_iNumFaces * 3 + 2] = c;
	//			m_Map.m_iNumFaces++;
	//			break;
	//		}
	//	}
	//}
	//if (vislbeIB.size() != 0)
	//{
	//	g_pImmediateContext->UpdateSubresource(m_Map.m_pIndexBuffer, 0 ,NULL, &vislbeIB.at(0), 0,0);
	//}
	//else
	//{
	//	m_Map.m_iNumFaces = 0;
	//}

	if (m_MinMap.Begin(g_pImmediateContext))
	{
		m_Map.SetMatrix(NULL,
			&m_TopCamera.m_matView,
			&m_TopCamera.m_matProj);
		m_Map.Render(g_pImmediateContext);


		Matrix matWorld;
		matWorld._41 = m_TopCamera.m_vCameraPos.x;
		matWorld._42 = m_TopCamera.m_vCameraPos.y;
		matWorld._43 = m_TopCamera.m_vCameraPos.z;



		m_BoxShape.SetMatrix(NULL,
			&m_TopCamera.m_matView,
			&m_TopCamera.m_matProj);
		m_BoxShape.Render(g_pImmediateContext);
		
		m_MinMap.End(g_pImmediateContext);

				// 미니맵 정중앙 배치
		//Vector3 vPos = m_BoxShape.m_vPos- m_BoxShape.m_vLook;
		//vPos.y = 30.0f;
		//m_TopCamera.CreateViewMatrix(
		//	vPos,
		//	m_BoxShape.m_vPos);

		/*m_pMainCamera->DrawFrustum(g_pImmediateContext,
			&m_TopCamera.m_matView,
			&m_TopCamera.m_matProj);*/

	}

	

	m_Map.SetMatrix(NULL,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_Map.m_cbData.vColor[0] = m_pMainCamera->m_vLook.x;
	m_Map.m_cbData.vColor[1] = m_pMainCamera->m_vLook.y;
	m_Map.m_cbData.vColor[2] = m_pMainCamera->m_vLook.z;
	m_Map.Render(g_pImmediateContext);

	m_BoxShape.SetMatrix(NULL,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_BoxShape.Render(g_pImmediateContext);

	m_MinMap.SetMatrix(NULL,
		NULL, //&m_pMainCamera->m_matView,
		NULL); //&m_pMainCamera->m_matProj);
	m_MinMap.Render(g_pImmediateContext);


	return true;
}
bool main::PostRender()
{
	TCore::PostRender();
	return true;
}
bool main::Release()
{
	m_MinMap.Release();
	m_Map.Release();
	m_BoxShape.Release();
	m_PlaneShape.Release();

	return true;
}