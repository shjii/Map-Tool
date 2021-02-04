#include "main.h"
void main::SetRasterizerState()
{
	HRESULT hr;
	if (m_pRS != nullptr) m_pRS->Release();
	D3D11_RASTERIZER_DESC rdesc;
	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = m_FillMode;
	rdesc.CullMode = m_CullMode;
	hr = m_pd3dDevice->CreateRasterizerState(&rdesc, &m_pRS);
	if (FAILED(hr))
	{
		return;
	}
}

void main::CompilerCheck(ID3DBlob* pErrorMsgs)
{
	C_STR szMsg = (char*)pErrorMsgs->GetBufferPointer();
	T_STR szError = to_mw(szMsg);
	MessageBox(NULL, szError.c_str(), L"ERROR", MB_OK);
}
bool main::Init()
{
	HRESULT hr;

	m_FillMode = D3D11_FILL_SOLID;
	m_CullMode = D3D11_CULL_BACK;
	SetRasterizerState();

	D3D11_RASTERIZER_DESC rdesc;
	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = D3D11_FILL_WIREFRAME;
	rdesc.CullMode = D3D11_CULL_BACK;
	hr = m_pd3dDevice->CreateRasterizerState(&rdesc, &m_pRSSolidBack);
	if (FAILED(hr))
	{
		return false;
	}	
	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = D3D11_FILL_WIREFRAME;
	rdesc.CullMode = D3D11_CULL_BACK;
	hr = m_pd3dDevice->CreateRasterizerState(&rdesc, &m_pRSWireBack);
	if (FAILED(hr))
	{
		return false;
	}
	m_VertexList.resize(4);
	m_VertexList[0] = { SVertex3(-0.5f, 0.5f, 0.5f), SVertex3(0,0,0) };
	m_VertexList[1] = { SVertex3(0.5f, 0.5f, 0.5f), SVertex3(0,0,0) };
	m_VertexList[2] = { SVertex3(-0.5f, -0.5f, 0.5f), SVertex3(0,0,0) };
	m_VertexList[3] = { SVertex3(0.5f, -0.5f, 0.5f), SVertex3(0,0,0) };

	//P_VERTEX v = { TVertex3(-0.5f, 0.5f, 0.5f), TVertex3(0,0,0) };
	//m_VertexList.emplace(m_VertexList.end(), v);
	//m_VertexList.emplace_back(TVertex3(0.5f, 0.5f, 0.5f),
	//							TVertex3(0.0f, 0.0f, 0.0f));
	//m_VertexList.emplace_back(TVertex3(-0.5f, -0.5f, 0.5f));
	//m_VertexList.emplace_back(TVertex3(0.5f, -0.5f, 0.5f));
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(SDataCB);
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_cbData;
	hr = m_pd3dDevice->CreateBuffer(&bd, NULL, &m_pConstantBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	// vertex buffer
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(P_VERTEX) * m_VertexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_VertexList.at(0);
	hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	m_IndexList.resize(6);
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;
	m_IndexList[2] = 2;
	m_IndexList[3] = 2;
	m_IndexList[4] = 1;
	m_IndexList[5] = 3;

	// index buffer
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_IndexList.at(0);
	hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	//shader
	ID3DBlob* pVSObj;
	ID3DBlob* pPSObj;
	ID3DBlob* pErrorMsgs;
	hr = D3DCompileFromFile(L"vs.txt", NULL, NULL, "VS", "vs_5_0", 0, 0, &pVSObj, &pErrorMsgs);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs);
		return false;
	}
	hr = m_pd3dDevice->CreateVertexShader(pVSObj->GetBufferPointer(), pVSObj->GetBufferSize(), NULL, &m_pVertexShader);
	hr = D3DCompileFromFile(L"PS.txt", NULL, NULL, "PS", "ps_5_0", 0, 0, &pPSObj, &pErrorMsgs);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs);
		return false;
	}
	hr = m_pd3dDevice->CreatePixelShader(pPSObj->GetBufferPointer(), pPSObj->GetBufferSize(), NULL, &m_pPixelShader);

	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
	hr = m_pd3dDevice->CreateInputLayout(
		layout,
		iNumElement,
		pVSObj->GetBufferPointer(),
		pVSObj->GetBufferSize(),
		&m_pInputLayout
	);
	return true;
}
bool main::Frame()
{
	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		m_FillMode = D3D11_FILL_WIREFRAME;
		SetRasterizerState();
		//m_pd3dContext->RSSetState(m_pRSWireBack);
	}
	if (g_Input.GetKey('9') == KEY_PUSH)
	{
		m_FillMode = D3D11_FILL_SOLID;
		SetRasterizerState();
		//m_pd3dContext->RSSetState(m_pRSSolidBack);
	}
	if (g_Input.GetKey('8') == KEY_PUSH)
	{
		m_CullMode = D3D11_CULL_BACK;
		SetRasterizerState();
		//m_pd3dContext->RSSetState(m_pRSWireBack);
	}
	if (g_Input.GetKey('7') == KEY_PUSH)
	{
		m_CullMode = D3D11_CULL_FRONT;
		SetRasterizerState();
		//m_pd3dContext->RSSetState(m_pRSSolidBack);
	}
	D3D11_MAPPED_SUBRESOURCE mr;
	HRESULT hr = m_pd3dContext->Map(m_pConstantBuffer, 0 , D3D11_MAP_WRITE_DISCARD,0, &mr);
	if (SUCCEEDED(hr))
	{
		SDataCB* pData = (SDataCB*)mr.pData;
		pData->vColor[0] = cosf(g_fGameTimer);
		pData->vColor[1] = sinf(g_fGameTimer);
		pData->vColor[2] = 1.0f - cosf(g_fGameTimer);
		pData->vColor[3] = 1;
		pData->vTime[0] = cosf(g_fGameTimer)*0.5f + 0.5f;
		pData->vTime[1] = g_fGameTimer;
		m_pd3dContext->Unmap(m_pConstantBuffer, 0);
	}
	return true;
}

bool main::Render()
{
	UINT iStride = sizeof(P_VERTEX);
	UINT iOffset = 0;
	m_pd3dContext->IASetVertexBuffers(0, 1,&m_pVertexBuffer, &iStride, &iOffset);
	m_pd3dContext->IASetIndexBuffer(m_pIndexBuffer,DXGI_FORMAT_R32_UINT, 0);
	m_pd3dContext->IASetInputLayout(m_pInputLayout);
	m_pd3dContext->VSSetConstantBuffers(0,1,&m_pConstantBuffer);
	m_pd3dContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pd3dContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pd3dContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dContext->RSSetState(m_pRS);
	//m_pd3dContext->Draw(m_VertexList.size(), 0);
	m_pd3dContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}
bool main::Release()
{
	m_pRS->Release();
	m_pRSSolidBack->Release();
	m_pRSWireBack->Release();
	m_pConstantBuffer->Release();
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pInputLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
	return true;
}
