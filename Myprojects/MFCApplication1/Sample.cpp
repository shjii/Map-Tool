#include "pch.h"
#include "Sample.h"

bool Sample::Init()
{
	m_EditorData.Radius = 10.0f;
	m_EditorData.Speed = 10.0f;
	m_EditorData.mapEditorB = UP;

	//m_pObj.Init();

	//m_pObj.m_pMainCamera = m_pMainCamera;
	m_MinMap.Width = 512;
	m_MinMap.Height = 512;

	m_MinMap.Create(g_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/tileA.jpg");

	m_Textrue.Create(g_pd3dDevice, L"vs.txt", L"ps.txt", L"");
	m_BlendingTextrue.Create(g_pd3dDevice, L"vs.txt", L"ps.txt", L"");
	m_BlendingTextrue.m_pSRV = m_BlendingTextrue.StagingCopyTextureFromSV(g_pd3dDevice, g_pImmediateContext, m_Map);

	m_MultiTextureSRV.resize(4);

	SetLayer();
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
		//m_Map->Frame();
	}	
	if (g_Input.GetKey(VK_MBUTTON) == KEY_PUSH || (g_Input.GetKey(VK_MBUTTON) == KEY_HOLD && ObjPinckB))
	{
		m_Mouse.RayFrame(m_pMainCamera->m_matWorld, m_pMainCamera->m_matView, m_pMainCamera->m_matProj);
		BoolColl = true;
	}
	if (g_Input.GetKey(VK_MBUTTON) == KEY_UP)
	{
		m_Textrue.m_pSRV = m_Textrue.StagingCopyTextureFromSV(g_pd3dDevice, g_pImmediateContext, m_Map);
	}
	if (!ObjPinckB)
	{
		ObjPinck = nullptr;
	}

	for (auto data : m_SRT)
	{
		data.m_pObj->Frame();
	}
	return true;
}
bool Sample::Render()
{


	if (m_Map != nullptr) 
	{
		if (m_MinMap.Begin(g_pImmediateContext))
		{
			m_Map->SetMatrix(NULL,
				&m_TopCamera.m_matView,
				&m_TopCamera.m_matProj);
			g_pImmediateContext->PSSetShaderResources(2, 1, m_BlendingTextrue.m_pSRV.GetAddressOf());
			g_pImmediateContext->PSSetShaderResources(3, 4, m_MultiTextureSRV[0].GetAddressOf());
			m_Map->Render(g_pImmediateContext);
			m_MinMap.End(g_pImmediateContext);
		}

		for (auto data : m_SRT)
		{
			for (int i = 0; i < data.m_MatrixList.size(); i++)
			{
				Matrix mat;
				mat = Matrix::Transform(data.m_MatrixList[i], data.m_Quaternion[i]);
				data.m_pObj->SetMatrix(&mat, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
				data.m_pObj->Render(g_pImmediateContext);
			}
		}

		m_Map->SetMatrix(NULL,
			&m_pMainCamera->m_matView,
			&m_pMainCamera->m_matProj);


			m_Map->SDxObject::Update(g_pImmediateContext);
			ID3D11Buffer* vb[2] = { m_Map->m_pVertexBuffer.Get(),	m_Map->m_pRC.Get() };
			UINT stride[2] = { sizeof(PNCT_VERTEX), sizeof(RC) };
			UINT offset[2] = { 0, 0 };
			g_pImmediateContext->IASetVertexBuffers(0, 2, vb, stride, offset);
			g_pImmediateContext->PSSetShaderResources(2, 1, m_BlendingTextrue.m_pSRV.GetAddressOf());
			g_pImmediateContext->PSSetShaderResources(3, 4, m_MultiTextureSRV[0].GetAddressOf());
			g_pImmediateContext->IASetIndexBuffer(m_Map->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			g_pImmediateContext->IASetInputLayout(m_Map->m_pInputLayout.Get());
			g_pImmediateContext->VSSetConstantBuffers(0, 1, m_Map->m_pConstantBuffer.GetAddressOf());
			g_pImmediateContext->PSSetConstantBuffers(0, 1, m_Map->m_pConstantBuffer.GetAddressOf());
			g_pImmediateContext->VSSetConstantBuffers(1, 1, m_ConstantBuffer.GetAddressOf());
			g_pImmediateContext->VSSetShader(m_Map->m_pVertexShader.Get(), NULL, 0);
			g_pImmediateContext->PSSetShader(m_Map->m_pPixelShader.Get(), NULL, 0);
			g_pImmediateContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_Map->m_iTopology);
			if (m_Map->m_pTexture != nullptr)
			{
				g_pImmediateContext->PSSetShaderResources(0, 1,
					&m_Map->m_pTexture->m_pTextureSRV);
			}

			if (m_Map->m_pIndexBuffer == nullptr)
			{
				g_pImmediateContext->Draw(m_Map->m_VertexList.size(), 0);
			}
			else
			{
				g_pImmediateContext->DrawIndexed(m_Map->m_IndexList.size(), 0, 0);
			}

			// юс╫ц 
			Matrix a;
			a._41 = 1.5f;

			m_MinMap.SetMatrix(NULL,
				NULL, 
				NULL);
			m_MinMap.Render(g_pImmediateContext);
			m_Textrue.SetMatrix(NULL,
				NULL,
				&a);
			m_Textrue.Render(g_pImmediateContext);

		if (BoolColl == true)
		{
			BoolColl = false;
			float Max = 99999;
			float f = 5.0f;
			Vector3 Pick;
			for (int i = 0; i < m_QuadTree.m_LeafNodeList.size(); i++)
			{
				//if(m_Mouse.OBBtoRay(&m_QuadTree.m_LeafNodeList[i]->m_Box, m_Mouse.Orig, m_Mouse.Dir));
				if (m_Mouse.SphereToRay(&m_QuadTree.m_LeafNodeList[i]->m_Sphere, m_Mouse.Orig, m_Mouse.Dir))
				{
					if (GetIntersection(m_QuadTree.m_LeafNodeList[i]))
					{
						float fDistance = (m_Mouse.Orig - m_Mouse.m_vIntersection).Length();
						if (fDistance < Max)
						{
							Pick = m_Mouse.m_vIntersection;
							Max = fDistance;
							BoolColl = true;
						}
					}
				}
			}
			if (BoolColl)
			{
				m_PinkList.clear();
				if (objp)
				{
					switch (objEd)
					{
					case 0 :
					{
						ObjPinckB = false;
						Matrix mat;
						
						mat._11 = m_Scale.x;
						mat._22 = m_Scale.y;
						mat._33 = m_Scale.z;
						mat._41 = Pick.x;
						mat._42 = Pick.y;
						mat._43 = Pick.z;
						bool end = true;
						for (auto data : m_SRT)
						{
							if (data.name == objname)
							{
								data.m_MatrixList.push_back(mat);
								data.m_Quaternion.push_back(m_baseQuaterniion);
								end = false;
								break;
							}
						}
						if (end)
						{
							SRT srt;
							srt.name = objname;
							srt.m_MatrixList.push_back(mat);
							srt.m_Quaternion.push_back(m_baseQuaterniion);
							srt.m_pObj = new SModel;
							srt.m_pObj->Build(objname, m_pMainCamera);
							m_SRT.push_back(srt);
						}
					}break;
					case 1 :
					{
						ObjPinckB = false;
						S_BOX Box;

						for (auto data : m_SRT)
						{
							for (int i = 0; i < data.m_MatrixList.size(); i++)
							{
								Box = data.m_pObj->m_Box;

								Matrix a = data.m_MatrixList[i];

								Box.vMax = Vector3::Transform(data.m_pObj->m_Box.vMax, a);
								Box.vMin = Vector3::Transform(data.m_pObj->m_Box.vMin, a);
								Box.vCenter = Vector3::Transform(data.m_pObj->m_Box.vCenter, a);

								Box.fExtent[0] = Box.vMax.x - Box.vCenter.x;
								Box.fExtent[1] = Box.vMax.y - Box.vCenter.y;
								Box.fExtent[2] = Box.vMax.z - Box.vCenter.z;

								if (m_Mouse.OBBtoRay(&Box, m_Mouse.Orig, m_Mouse.Dir))
								{
									data.m_MatrixList.erase(data.m_MatrixList.begin() + i);
									data.m_Quaternion.erase(data.m_Quaternion.begin() + i);
									break;
								}
							}
						}

					}break;
					case 2:
					{
						for (auto data : m_SRT)
						{
							S_BOX Box;
							for (int i = 0; i < data.m_MatrixList.size(); i++)
							{
								Box = data.m_pObj->m_Box;

								Matrix a = data.m_MatrixList[i];

								Box.vMax = Vector3::Transform(data.m_pObj->m_Box.vMax, a);
								Box.vMin = Vector3::Transform(data.m_pObj->m_Box.vMin, a);
								Box.vCenter = Vector3::Transform(data.m_pObj->m_Box.vCenter, a);

								Box.fExtent[0] = Box.vMax.x - Box.vCenter.x;
								Box.fExtent[1] = Box.vMax.y - Box.vCenter.y;
								Box.fExtent[2] = Box.vMax.z - Box.vCenter.z;

								if (m_Mouse.OBBtoRay(&Box, m_Mouse.Orig, m_Mouse.Dir))
								{
									ObjPinckB = true;
									ObjPinck = &data.m_MatrixList[i];
									objQuate = &data.m_Quaternion[i];
									data.m_MatrixList[i]._41 = Pick.x;
									data.m_MatrixList[i]._42 = Pick.y;
									data.m_MatrixList[i]._43 = Pick.z;
									break;
								}
							}
						}
					}break;
					default:
						break;
					}
				}
				else
				{
					ObjPinckB = false;
					for (int i = 0; i < m_Map->m_VertexList.size(); i++)
				{
					float fDist = (m_Map->m_VertexList[i].p - Pick).Length();
					if (fDist < m_EditorData.Radius)
					{
						switch (m_EditorData.mapEditorB)
						{
						case UP:
						{
							m_Map->m_VertexList[i].p.y = m_Map->m_VertexList[i].p.y + m_EditorData.Speed;
							m_MapData.fyList[i] = m_Map->m_VertexList[i].p.y;

						}break;
						case DOWN:
						{
							m_Map->m_VertexList[i].p.y = m_Map->m_VertexList[i].p.y - m_EditorData.Speed;
							m_MapData.fyList[i] = m_Map->m_VertexList[i].p.y;
						}break;
						case NORMAL:
						{
							float a = m_Map->m_VertexList[i].p.y;
							for (int i = 0; i < m_Map->m_VertexList.size(); i++)
							{
								m_Map->m_VertexList[i].p.y = a;
								m_MapData.fyList[i] = m_Map->m_VertexList[i].p.y;
							}
							break;
						}break;
						case ELayer0:
						{
							if (m_PinkList.size() == 0)
							{
								m_PinkList.push_back(Pick);
								Vector3 a = { m_EditorData.Radius, 0.0f, 0.0f };
								m_PinkList.push_back(a);
								m_BlendingTextrue.m_pSRV = m_BlendingTextrue.StagingCopyTextureFromSV(g_pd3dDevice, g_pImmediateContext, 0, m_PinkList, m_Map);
							}
						}break;
						case ELayer1:
						{
							if (m_PinkList.size() == 0)
							{
								m_PinkList.push_back(Pick);
								Vector3 a = { m_EditorData.Radius, 0.0f, 0.0f };
								m_PinkList.push_back(a);
								m_BlendingTextrue.m_pSRV = m_BlendingTextrue.StagingCopyTextureFromSV(g_pd3dDevice, g_pImmediateContext, 1, m_PinkList, m_Map);
							}
						}break;
						case ELayer2:
						{
							if (m_PinkList.size() == 0)
							{
								m_PinkList.push_back(Pick);
								Vector3 a = { m_EditorData.Radius, 0.0f, 0.0f };
								m_PinkList.push_back(a);
								m_BlendingTextrue.m_pSRV = m_BlendingTextrue.StagingCopyTextureFromSV(g_pd3dDevice, g_pImmediateContext, 2, m_PinkList, m_Map);
							}
						}break;
						case ELayer3:
						{
							if (m_PinkList.size() == 0)
							{
								m_PinkList.push_back(Pick);
								Vector3 a = { m_EditorData.Radius, 0.0f, 0.0f };
								m_PinkList.push_back(a);
								m_BlendingTextrue.m_pSRV = m_BlendingTextrue.StagingCopyTextureFromSV(g_pd3dDevice, g_pImmediateContext, 3, m_PinkList, m_Map);
							}
						}break;
						}
					}
				}
				}
			}
			m_Map->UpdateVertexBuffer(g_pImmediateContext, &m_Map->m_VertexList.at(0), 0);
			BoolColl = false;
		}
	}
	return true;
}
bool Sample::Release()
{
	for (auto data : m_SRT)
	{
		data.m_pObj->Release();
	}
	if(m_Map != nullptr)m_Map->Release();
	//m_pObj.Release();
	m_QuadTree.Release();
	return true;
}

bool Sample::Build(int tel, int cel, int ces, wstring tex)
{
	if (m_Map != nullptr)
	{
		m_Map->Release();
		m_BlendingTextrue.m_pSRV = m_BlendingTextrue.StagingCopyTextureFromSV(g_pd3dDevice, g_pImmediateContext, nullptr);
	}
	m_Map = nullptr;
	SMapDesc desc;
	desc.iNumCols = tel * cel + 1;
	desc.iNumRows = tel * cel + 1;
	desc.fCellDistance = ces;
	desc.fScaleHeight = 1.0f;
	desc.szTexFile = tex;
	desc.szPS = L"map.hlsl";
	desc.szVS = L"map.hlsl";
	m_Map = new SMap;
 	m_Map->CreateMap(g_pd3dDevice, g_pImmediateContext, desc);
	m_TopCamera.CreateViewMatrix({ 0,100.0f,-1.0f }, { 0,0,0 });
	m_TopCamera.CreateOrthographic(
		desc.iNumCols * ces, desc.iNumRows * ces, 1.0f, 10000);
	m_TopCamera.Init();
	m_QuadTree.GetUpdata(m_pMainCamera);

	m_MapData.fTile = tel;
	m_MapData.fCell = cel;
	m_MapData.fCellSize = ces;
	m_MapData.fTexture = tex;
	m_MapData.fyList.resize(desc.iNumCols * desc.iNumRows);
	int a = sqrt(tel);
	int b = 1;
	while (2 < a)
	{
		b++;
		a /= 2;
	}
	if (tel == 1) b = 0;
	m_QuadTree.GetUpdata(m_pMainCamera);
	m_QuadTree.m_MaxDepth = b;
	m_QuadTree.Build(m_Map);
	

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(SRCData);
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, NULL, m_ConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_MAPPED_SUBRESOURCE mr;
	hr = g_pImmediateContext->Map(m_ConstantBuffer.Get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &mr);
	if (SUCCEEDED(hr))
	{
		SRCData* pData = (SRCData*)mr.pData;
		pData->vRc[0] = tel;
		pData->vRc[1] = cel;
		pData->vRc[2] = ces;
		pData->vRc[3] = tel * cel;

		g_pImmediateContext->Unmap(m_ConstantBuffer.Get(), 0);
	}
	//
	return true;
}

bool Sample::GetIntersection(SNode* pNode)
{
	// face list
	for (int face = 0; face < pNode->m_IndexList.size() / 3; face++)
	{
		Vector3 v0 = m_Map->m_VertexList[pNode->m_IndexList[face * 3 + 0]].p;
		Vector3 v1 = m_Map->m_VertexList[pNode->m_IndexList[face * 3 + 1]].p;
		Vector3 v2 = m_Map->m_VertexList[pNode->m_IndexList[face * 3 + 2]].p;

		Vector3 vEnd = m_Mouse.Orig + m_Mouse.Dir * 10000.0f;
		Vector3 vNormal = (v1 - v0).Cross(v2 - v0);
		vNormal.Normalize();

		if (m_Mouse.GetIntersection(m_Mouse.Orig, vEnd, vNormal, v0, v1, v2))
		{
			if (m_Mouse.PointInPolygon(m_Mouse.m_vIntersection, vNormal, v0, v1, v2))
			{
				return true;
			}
		}
	}

	return false;
}
bool Sample::SetEditor()
{
	FileIO.Load(&m_MapData,g_pd3dDevice, g_pImmediateContext);
	if (m_Map != nullptr)m_Map->Release();
	Build(m_MapData.fTile, m_MapData.fCell, m_MapData.fCellSize, m_MapData.fTexture);
	m_BlendingTextrue.m_pSRV = m_MapData.ResourceView;
	m_BlendingTextrue.pTex2D = m_MapData.btext;
	SetLayer();
	for (int i = 0; i < m_Map->m_VertexList.size(); i++)
	{
		m_Map->m_VertexList[i].p.y = m_MapData.fyList[i];
	}
	m_Map->UpdateVertexBuffer(g_pImmediateContext, &m_Map->m_VertexList.at(0), 0);

	return true;
}
bool Sample::SetLayer()
{
	m_MultiTextureSRV[0].Attach(g_TextMgr.SRVLoad(g_pd3dDevice, m_MapData.m_LayerList[0].c_str()));
	m_MultiTextureSRV[1].Attach(g_TextMgr.SRVLoad(g_pd3dDevice, m_MapData.m_LayerList[1].c_str()));
	m_MultiTextureSRV[2].Attach(g_TextMgr.SRVLoad(g_pd3dDevice, m_MapData.m_LayerList[2].c_str()));
	m_MultiTextureSRV[3].Attach(g_TextMgr.SRVLoad(g_pd3dDevice, m_MapData.m_LayerList[3].c_str()));
	return true;
}

bool Sample::setSRT(Vector3 scale, Quaternion rotation)
{
	m_Scale = scale;
	m_baseQuaterniion = rotation;
	if (ObjPinckB)
	{
		ObjPinck->_11 = scale.x;
		ObjPinck->_22 = scale.y;
		ObjPinck->_33 = scale.z;
		objQuate->x = rotation.x;
		objQuate->y = rotation.y;
		objQuate->z = rotation.z;
	}
	return true;
}

Sample::Sample()
{
	m_MapData.m_LayerList.push_back(L"../../data/map/Terrain1_Mask.dds");
	m_MapData.m_LayerList.push_back(L"../../data/map/firem512.dds");
	m_MapData.m_LayerList.push_back(L"../../data/map/bul.dds");
	m_MapData.m_LayerList.push_back(L"../../data/map/lighting.dds");
}
Sample::~Sample()
{

}