#include "pch.h"
#include "ComputeShader.h"
template<typename T>
struct arrayDelete
{
	void operator() (T const* p)
	{
		delete[] p;
	}
};

void ComputeShader::InitWork(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BufType SelectData, ID3D11ShaderResourceView* mapVIew)
{
	CreateStructuredBuffer(pDevice, sizeof(BufType), 1, &SelectData, m_pSelectBuffer.GetAddressOf(), SelectData);
	CreateBufferSRV(pDevice, m_pSelectBuffer.Get(), m_pSelectBufferSRV.GetAddressOf());
	CreateTextureUAV(pDevice, m_iTextureSizeX, m_iTextureSizeY);
	CreateComputeShader(pDevice, L"CS.txt", "CS", m_pCS.GetAddressOf());

	ID3D11ShaderResourceView* arraySRV[3] =
	{
		mapVIew,
		m_pAlphaTextureSRVCopy.Get(),
		m_pSelectBufferSRV.Get()
	};

	RunComputeShader(pContext, m_pCS.Get(), arraySRV, m_pAlphaTextureUAV.GetAddressOf(), 3, m_iTextureSizeX / 32 , m_iTextureSizeY / 32 , 0);

	pContext->CopyResource(m_pAlphaTextureCopy.Get(), m_pAlphaTexture.Get());
}
HRESULT	ComputeShader::CreateComputeShader(ID3D11Device* pDevice, LPCWSTR srcfile, LPCSTR functinName, ID3D11ComputeShader** ppShaderOut)
{
	HRESULT hr = S_OK;
	*ppShaderOut = LoadComputeShaderFile(pDevice, srcfile, nullptr, "CS");
	return hr;
}
void	ComputeShader::RunComputeShader(ID3D11DeviceContext*, ID3D11ComputeShader*, ID3D11ShaderResourceView**, ID3D11UnorderedAccessView**, int, UINT, UINT, UINT)
{

}
HRESULT ComputeShader::CreateStructuredBuffer(ID3D11Device* pDevice, UINT uElementSize, UINT uCount, VOID* pInitData, ID3D11Buffer** ppBufOut, BufType SelectData)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS; // 필수
	bd.ByteWidth = sizeof(BufType) * 1;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.StructureByteStride = sizeof(BufType);
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &SelectData;
	HRESULT hr = pDevice->CreateBuffer(&bd, &initData, m_pSelectBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}
HRESULT ComputeShader::CreateBufferSRV(ID3D11Device* pDevice, ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRVOut)
{
	D3D11_BUFFER_DESC desc;
	pBuffer->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));

	srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvd.BufferEx.FirstElement = 0;
	srvd.Format = DXGI_FORMAT_UNKNOWN;
	srvd.BufferEx.NumElements = desc.ByteWidth / desc.StructureByteStride;
	HRESULT hr = pDevice->CreateShaderResourceView(pBuffer, &srvd, ppSRVOut);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}
HRESULT ComputeShader::CreateTextureUAV(ID3D11Device* pDevice, int iWidth, int iHeight)
{
	HRESULT hr = S_OK;
	std::shared_ptr<int> buf(new int[iWidth*iHeight], arrayDelete<int>());

	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = iWidth;
	td.Height = iHeight;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE; // 필수
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	// d3d11_usage_staging // cpu
	// d3d11_usage_dynamic // cpu
	// UpdateSubresource  // gup
	// pSysMem   // init
	data.pSysMem = (void*)buf.get();
	data.SysMemPitch = iWidth * 4;
	//data.SysMemSlicePitch = iWidth* iHeight*4; // 3D
	hr = pDevice->CreateTexture2D(&td, &data, m_pAlphaTexture.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavd;
	ZeroMemory(&uavd, sizeof(uavd));
	uavd.Format = td.Format;
	uavd.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavd.Texture2D.MipSlice = 0;
	hr = pDevice->CreateUnorderedAccessView(m_pAlphaTexture.Get(), &uavd,
		m_pAlphaTextureUAV.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.Format = td.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = 1;
	hr = pDevice->CreateShaderResourceView(m_pAlphaTexture.Get(), &srvd,
		m_pAlphaTextureSRV.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	// copy
	hr = pDevice->CreateTexture2D(&td, &data,
		m_pAlphaTextureCopy.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	hr = pDevice->CreateShaderResourceView(m_pAlphaTextureCopy.Get(),
		&srvd, m_pAlphaTextureSRVCopy.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}