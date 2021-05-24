#pragma once
#include "TStd.h"

struct BufType
{
	Vector3 vPickPos;
	float	fRadius;
	int		iIndex;
	float	fTexWidth;
	float	fTexHeight;
};

class ComputeShader
{
public:
	int m_iTextureSizeX = 1024;
	int m_iTextureSizeY = 1024;
	

	ComPtr<ID3D11ComputeShader>	m_pCS;
	ComPtr<ID3D11Buffer>		m_pSelectBuffer;
	ComPtr<ID3D11Texture2D>  m_pAlphaTexture;
	ComPtr<ID3D11Texture2D>  m_pAlphaTextureCopy;
	ComPtr<ID3D11ShaderResourceView>  m_pAlphaTextureSRV;
	ComPtr<ID3D11UnorderedAccessView>  m_pAlphaTextureUAV;
	ComPtr<ID3D11ShaderResourceView>  m_pAlphaTextureSRVCopy;
	ComPtr<ID3D11ShaderResourceView>  m_pSelectBufferSRV;
public:
	void	InitWork(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BufType SelectData, ID3D11ShaderResourceView* mapVIew);
	HRESULT	CreateComputeShader(ID3D11Device* pDevice, LPCWSTR srcfile, LPCSTR functinName, ID3D11ComputeShader** ppShaderOut);
	void	RunComputeShader(ID3D11DeviceContext*, ID3D11ComputeShader*, ID3D11ShaderResourceView**, ID3D11UnorderedAccessView**, int , UINT, UINT, UINT);
	HRESULT CreateStructuredBuffer(ID3D11Device* pDevice,UINT uElementSize,	UINT uCount,VOID* pInitData,ID3D11Buffer** ppBufOut, BufType SelectData);
	HRESULT CreateBufferSRV(ID3D11Device* pDevice, ID3D11Buffer* pBuffer,ID3D11ShaderResourceView** ppSRVOut);
	HRESULT CreateTextureUAV(ID3D11Device* pDevice, int iWidth, int iHeight);
};

