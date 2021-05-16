#pragma once
#include "SDxObject.h"
#include "SShape.h"
#include "SMap.h"
enum Layer
{
	Layer0,
	Layer1,
	Layer2,
	Layer3,
};
class SText : public SShapePlane
{
public:
	ComPtr<ID3D11ShaderResourceView> m_pSRV;
	SShapePlane a;
	ComPtr<ID3D11Texture2D> pTex2D;
	ComPtr<ID3D11Texture2D> pStaging2D;
public:
	bool CreateVertexData();
	bool CreateIndexData();
	bool Release();
	bool Render(ID3D11DeviceContext*	pd3dContext);
	bool PostRender(ID3D11DeviceContext*	pd3dContext);
	ID3D11ShaderResourceView*	StagingCopyTextureFromSV(ID3D11Device* pDevice, ID3D11DeviceContext*   pImmediateContext, SMap* map = nullptr);
	ID3D11ShaderResourceView*	StagingCopyTextureFromSV(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext, int index, vector<Vector3> list, SMap* map);
	virtual bool	Create(ID3D11Device* pDevice, T_STR szVS, T_STR szPS, T_STR	szTex);
	void			WriteDotPixel(ID3D11DeviceContext*   pImmediateContext, ID3D11Texture2D* pTexDest, SMap* map);
	void			WriteDotPixel(ID3D11DeviceContext*   pImmediateContext, ID3D11Texture2D* pTexDest, int index, vector<Vector3> list, SMap* map);
	void			WriteDotPixel(ID3D11DeviceContext*   pImmediateContext, ID3D11Texture2D* pTexDest);
};

