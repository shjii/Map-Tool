#include "pch.h"
#include "SText.h"

bool	SText::Create(ID3D11Device* pDevice, T_STR szVS, T_STR szPS, T_STR	szTex)
{
	SDxObject::Create(pDevice, szVS, szPS, szTex);
	HRESULT hr;
	//ID3D11Texture2D* pTexture = nullptr;
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = 1024;
	texDesc.Height = 1024;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET |
		D3D11_BIND_SHADER_RESOURCE;
	hr = g_pd3dDevice->CreateTexture2D(&texDesc, NULL, pTex2D.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	hr = g_pd3dDevice->CreateShaderResourceView(pTex2D.Get(),
		NULL,
		m_pSRV.GetAddressOf());
	texDesc.Usage = D3D11_USAGE_STAGING;
	texDesc.BindFlags = 0;
	texDesc.MiscFlags = 0;
	texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	if (FAILED(hr = pDevice->CreateTexture2D(&texDesc, NULL, pStaging2D.GetAddressOf())))
	{
		H(hr);
		return nullptr;
	}
	//if (pTexture) pTexture->Release();
	return hr;
}

bool    SText::CreateVertexData()
{
	m_VertexList.resize(4);
	m_VertexList[0] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(Vector3(-0.5f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(Vector3(-0.5f, 0.5f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 0.0f, 0.0f));
	m_VertexList[3] = PNCT_VERTEX(Vector3(-1.0f, 0.5f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f));

	return true;
}
bool    SText::CreateIndexData()
{
	SShapePlane::CreateIndexData();
	return true;
}
bool  SText::Release()
{
	return TObject::Release();
}
bool SText::Render(ID3D11DeviceContext*	pd3dContext)
{
	SShapePlane::Render(pd3dContext);
	return true;
}
bool SText::PostRender(ID3D11DeviceContext*	pd3dContext)
{
	pd3dContext->PSSetShaderResources(0, 1, m_pSRV.GetAddressOf());
	TObject::PostRender(pd3dContext);
	return true;
}
ID3D11ShaderResourceView*	SText::StagingCopyTextureFromSV(ID3D11Device* pDevice, ID3D11DeviceContext*   pImmediateContext,SMap* map)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = 1024;
	desc.Height = 1024;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	//if (FAILED(hr = pDevice->CreateTexture2D(&desc, NULL, pTex2D.GetAddressOf())))
	//{
	//	H(hr);
	//	return nullptr;
	//}
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	if (FAILED(hr = pDevice->CreateTexture2D(&desc, NULL, pStaging2D.GetAddressOf())))
	{
		H(hr);
		return nullptr;
	}
	pImmediateContext->CopyResource(pStaging2D.Get(), pTex2D.Get());
	if (map != nullptr)
	{
		WriteDotPixel(pImmediateContext, pStaging2D.Get(), map);
	}
	else
	{
		WriteDotPixel(pImmediateContext, pStaging2D.Get());
	}
	pImmediateContext->CopyResource(pTex2D.Get(), pStaging2D.Get());
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = desc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = desc.MipLevels;
	ID3D11ShaderResourceView*	pTexSRV;
	
	if (FAILED(hr = pDevice->CreateShaderResourceView(pTex2D.Get(), &SRVDesc, &pTexSRV)))
	{
		H(hr);
		//pTex2D->Release();
		return nullptr;
	}
	//pTex2D->Release();
	return pTexSRV;
}

ID3D11ShaderResourceView*	SText::StagingCopyTextureFromSV(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext,  int index, vector<Vector3> list, SMap* map)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = 1024;
	desc.Height = 1024;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	//if (FAILED(hr = pDevice->CreateTexture2D(&desc, NULL, pTex2D.GetAddressOf())))
	//{
	//	H(hr);
	//	return nullptr;
	//}
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	if (FAILED(hr = pDevice->CreateTexture2D(&desc, NULL, pStaging2D.GetAddressOf())))
	{
		H(hr);
		return nullptr;
	}
	pImmediateContext->CopyResource(pStaging2D.Get() , pTex2D.Get());
	WriteDotPixel(pImmediateContext, pStaging2D.Get(), index, list , map);
	pImmediateContext->CopyResource(pTex2D.Get(), pStaging2D.Get());
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = desc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = desc.MipLevels;
	ID3D11ShaderResourceView*	pTexSRV;
	if (FAILED(hr = pDevice->CreateShaderResourceView(pTex2D.Get(), &SRVDesc, &pTexSRV)))
	{
		H(hr);
		//pTex2D->Release();
		return nullptr;
	}
	//pTex2D->Release();
	return pTexSRV;
}
void SText::WriteDotPixel(ID3D11DeviceContext*   pImmediateContext, ID3D11Texture2D* pTexDest, int index, vector<Vector3> list, SMap* map)
{
	D3D11_TEXTURE2D_DESC desc;
	pTexDest->GetDesc(&desc);

	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(pImmediateContext->Map((ID3D11Resource*)pTexDest,
		0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		float fMapWidth = map->m_iNumCellCols * map->m_fCellDistance;
		float fWidthRatio = (float)desc.Width / fMapWidth;
		Vector2 vCenter = Vector2((list[0].x + (fMapWidth / 2.0f)) * fWidthRatio,
			(-(list[0].z - (fMapWidth / 2.0f))) * fWidthRatio);
		float fRadius = list[1].x * fWidthRatio;

		for (UINT y = 0; y < desc.Height; y++)
		{
			for (UINT x = 0; x < desc.Width; x++)
			{
				bool c = false;
				Vector2 p = Vector2(x, y);
				float fDist = (p - vCenter).Length();
				if (fDist < fRadius)
				{
					float iRatio = 0.0f;
					int iTemp = 0;

					iRatio = 255;
					iTemp = *pDestBytes + (int)iRatio;
					if (iTemp > 255) iTemp = 255;
					pDestBytes += index;
					*pDestBytes = iTemp;
					c = true;
				}
				if (c)pDestBytes += (4 - index);
				else pDestBytes += 4;
				continue;
			}
		}
	}
	pImmediateContext->Unmap(pTexDest, 0);
}
void SText::WriteDotPixel(ID3D11DeviceContext*   pImmediateContext, ID3D11Texture2D* pTexDest, SMap* map)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC desc;
	pTexDest->GetDesc(&desc);
	float fRow = map->m_iNumCellRows / (float)desc.Width;
	float fCol = map->m_iNumCellCols / (float)desc.Height;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(pImmediateContext->Map((ID3D11Resource*)pTexDest, 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;

		for (UINT i = 1; i < desc.Height + 1; i++)
		{
			for (UINT j = 1; j < desc.Width + 1; j++)
			{
				int iRow = i * fRow;
				int iCow = j * fCol;
				float a = map->m_VertexList[iRow *map->m_iNumRows + iCow].p.y;
				*pDestBytes++ = a;
				*pDestBytes++ = a;
				*pDestBytes++ = a;
				*pDestBytes++ = 255;
			}
		}
		pImmediateContext->Unmap(pTexDest, 0);
	}
}
void	SText::WriteDotPixel(ID3D11DeviceContext*   pImmediateContext, ID3D11Texture2D* pTexDest)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC desc;
	pTexDest->GetDesc(&desc);

	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(pImmediateContext->Map((ID3D11Resource*)pTexDest, 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;

		for (UINT i = 1; i < desc.Height + 1; i++)
		{
			for (UINT j = 1; j < desc.Width + 1; j++)
			{
				*pDestBytes++ = 0;
				*pDestBytes++ = 0;
				*pDestBytes++ = 0;
				*pDestBytes++ = 0;
			}
		}
		pImmediateContext->Unmap(pTexDest, 0);
	}
}