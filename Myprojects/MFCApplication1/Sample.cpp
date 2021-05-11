#include "pch.h"
#include "Sample.h"

bool Sample::Init()
{
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
	return true;
}
bool Sample::Render()
{
	if (m_Map != nullptr) {
		g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_pImmediateContext->RSSetState(SDxState::m_pRS);
		g_pImmediateContext->PSSetSamplers(0, 1, &SDxState::m_pWrapLinear);
		g_pImmediateContext->OMSetDepthStencilState(SDxState::m_pDSS, 0);

		m_Map->SetMatrix(NULL,
			&m_pMainCamera->m_matView,
			&m_pMainCamera->m_matProj);

		m_Map->Render(g_pImmediateContext);

		for (int i = 0; i < m_QuadTree.m_LeafNodeLIst.size(); i++)
		{
			m_LineShape.Draw(g_pImmediateContext,
				m_Map->m_VertexList[m_QuadTree.m_LeafNodeLIst[i]->m_iCorner[0]].p
				, m_Map->m_VertexList[m_QuadTree.m_LeafNodeLIst[i]->m_iCorner[1]].p, Vector4(1, 0, 0, 1));
			
			m_LineShape.Draw(g_pImmediateContext,
				m_Map->m_VertexList[m_QuadTree.m_LeafNodeLIst[i]->m_iCorner[1]].p
				, m_Map->m_VertexList[m_QuadTree.m_LeafNodeLIst[i]->m_iCorner[2]].p, Vector4(1, 0, 0, 1));

			m_LineShape.Draw(g_pImmediateContext,
				m_Map->m_VertexList[m_QuadTree.m_LeafNodeLIst[i]->m_iCorner[2]].p
				, m_Map->m_VertexList[m_QuadTree.m_LeafNodeLIst[i]->m_iCorner[0]].p, Vector4(1, 0, 0, 1));

			m_LineShape.Draw(g_pImmediateContext,
				m_Map->m_VertexList[m_QuadTree.m_LeafNodeLIst[i]->m_iCorner[2]].p
				, m_Map->m_VertexList[m_QuadTree.m_LeafNodeLIst[i]->m_iCorner[1]].p, Vector4(1, 0, 0, 1));

			m_LineShape.Draw(g_pImmediateContext,
				m_Map->m_VertexList[m_QuadTree.m_LeafNodeLIst[i]->m_iCorner[1]].p
				, m_Map->m_VertexList[m_QuadTree.m_LeafNodeLIst[i]->m_iCorner[3]].p, Vector4(1, 0, 0, 1));

			m_LineShape.Draw(g_pImmediateContext,
				m_Map->m_VertexList[m_QuadTree.m_LeafNodeLIst[i]->m_iCorner[3]].p
				, m_Map->m_VertexList[m_QuadTree.m_LeafNodeLIst[i]->m_iCorner[2]].p, Vector4(1, 0, 0, 1));
		}


	}
	return true;
}
bool Sample::Release()
{
	if(m_Map != nullptr)m_Map->Release();
	m_QuadTree.Release();
	return true;
}

bool Sample::Build(int tel, int cel, int ces, wstring tex)
{
	SMapDesc desc;
	desc.iNumCols = sqrt(tel * cel) + 1;// m_Map.m_iNumCols;
	desc.iNumRows = sqrt(tel * cel) + 1;// m_Map.m_iNumRows;
	desc.fCellDistance = ces;
	desc.fScaleHeight = 1.0f;
	desc.szTexFile = tex;
	desc.szPS = L"ps.txt";
	desc.szVS = L"vs.txt";
	m_Map = new SMap;
 	m_Map->CreateMap(g_pd3dDevice, g_pImmediateContext, desc);

	m_QuadTree.GetUpdata(m_pMainCamera);

	int a = sqrt(tel);
	int b = 1;
	while (2 < a)
	{
		b++;
		a /= 2;
	}
	if (tel == 1) b = 0;
	m_QuadTree.m_MaxDepth = b;
	m_QuadTree.Build(m_Map);
	return true;
}
