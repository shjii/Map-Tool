#include "main.h"

/*
#define NUM_OBJECTS 10
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

void main::DrawObject(Matrix* pView, Matrix* pProj)
{
	for (int iBox = 0; iBox < NUM_OBJECTS; iBox++)
	{
		m_pObject[iBox].m_matWorld._42 =
			m_Map.GetHeightMap(m_pObject[iBox].m_matWorld._41,
				m_pObject[iBox].m_matWorld._43);

		m_BoxShape.SetMatrix(&m_pObject[iBox].m_matWorld,
			pView,
			pProj);
		// OBB와 프로스텀 박스의 제외처리( 걸쳐 있어도 TRUE가 됨. )
		if (m_pMainCamera->m_Frustum.CheckOBBInPlane(&m_pObject[iBox].m_sBox))
		{
			m_BoxShape.Render(g_pImmediateContext);
		}
	}
}
*/
//LRESULT	 main::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	if (m_pMainCamera == nullptr) return -1;
//	m_pMainCamera->WndProc(hWnd, message, wParam, lParam);
//	return -1;
//}

bool main::Init()
{
	HRESULT hr;

	m_Map.CreateHeightMap(g_pd3dDevice, g_pImmediateContext, L"../../data/map/heightMap513.bmp");

	SMapDesc desc;
	desc.iNumCols = m_Map.m_iNumCols;
	desc.iNumRows = m_Map.m_iNumRows;
	desc.fCellDistance = 1;
	desc.fScaleHeight = 10.0f;
	desc.szTexFile = L"../../data/map/grasshill.jpg";
	desc.szPS = L"ps.txt";
	desc.szVS = L"vs.txt";

	m_MinMap.Create(g_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/tileA.jpg");

	m_pMainCamera->Create(g_pd3dDevice);
	m_pMainCamera->CreateViewMatrix({ 0,100,-10 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_pMainCamera->CreateProjMatrix(1, 500, TBASIS_PI / 4.0f, fAspect);
	m_Map.CreateMap(g_pd3dDevice, g_pImmediateContext, desc);

	m_QuadTree.GetUpdata(m_pMainCamera);
	m_QuadTree.m_MaxDepth = 5;
	m_QuadTree.Build(&m_Map);
	///
	m_TopCamera.CreateViewMatrix({ 0,1000,-0.1f }, { 0,0,0 });
	m_TopCamera.CreateOrthographic(
		desc.iNumCols, desc.iNumRows, 1.0f, 1000);
	m_TopCamera.Init();
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
	if (g_Input.GetKey(VK_RBUTTON) == KEY_PUSH)
	{
		m_Mouse.RayFrame(m_pMainCamera->m_matWorld, m_pMainCamera->m_matView, m_pMainCamera->m_matProj);
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

	m_Map.UpdateIndexBuffer(g_pImmediateContext, &m_QuadTree.m_IndexList.at(0), m_QuadTree.Face);
	m_Map.m_IndexList = m_QuadTree.m_IndexList;
	m_Map.Frame();
	m_pMainCamera->Frame();
	return true;
}
bool main::Render()
{
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->RSSetState(SDxState::m_pRS);
	g_pImmediateContext->PSSetSamplers(0, 1, &SDxState::m_pWrapLinear);
	g_pImmediateContext->OMSetDepthStencilState(SDxState::m_pDSS, 0);

	if (m_MinMap.Begin(g_pImmediateContext))
	{
		m_Map.SetMatrix(NULL,
			&m_TopCamera.m_matView,
			&m_TopCamera.m_matProj);
		m_Map.Render(g_pImmediateContext);
		//m_QuadTree.Render(g_pImmediateContext);
		m_MinMap.End(g_pImmediateContext);
	//	Matrix matWorld;
	//	matWorld._41 = m_TopCamera.m_vCameraPos.x;
	//	matWorld._42 = m_TopCamera.m_vCameraPos.y;
	//	matWorld._43 = m_TopCamera.m_vCameraPos.z;
	}

	m_Map.SetMatrix(NULL,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_Map.Render(g_pImmediateContext);

	//m_QuadTree.Render(g_pImmediateContext);

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
	m_Map.Release();
	m_MinMap.Release();
	m_QuadTree.Release();
	return true;
}