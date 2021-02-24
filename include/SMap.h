#pragma once
#include "TObject.h"
struct SMapDesc
{
	int	iNumCols;
	int iNumRows;
	float fCellDistance;
	T_STR szTexFile;
	T_STR szVS;
	T_STR szPS;
};
class SMap : public TObject
{
public:
	SMapDesc  m_MapDesc;
	UINT m_iNumRows;
	UINT m_iNumCols;
	UINT m_iNumCellCols;
	UINT m_iNumCellRows;
	UINT m_iNumVertices;
	UINT m_iNumFaces;
	float m_fCellDistance;
	vector<float> m_fHeightList;
public:
	virtual float	GetHeight(UINT index);
	virtual bool	CreateMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,SMapDesc  desc);
	virtual bool    CreateVertexData() override;
	virtual bool    CreateIndexData()override;
	virtual bool	Frame() override;
	virtual bool	Postrender(ID3D11DeviceContext* pd3dContext);
public:
	SMap();
	virtual ~SMap();
};

