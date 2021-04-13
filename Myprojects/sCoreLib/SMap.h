#pragma once
#include "TObject.h"
struct SMapDesc
{
	int	iNumCols;
	int iNumRows;
	float fCellDistance;
	float fScaleHeight;
	T_STR szTexFile;
	T_STR szVS;
	T_STR szPS;
};
struct SNormalLookupTable
{
	int index[6];
	SNormalLookupTable()
	{
		index[0] = index[1] = index[2] = index[3] = index[4] = index[5] = -1;
	}
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
	virtual float	GetHeightMap(float fPosX, float fPosZ);
	virtual float	GetHeightMap(int row, int col);
	virtual float	Lerp(float fStart, float fEnd, float fTangent);
	virtual bool	CreateMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,SMapDesc  desc);
	virtual bool    CreateVertexData() override;
	virtual bool    CreateIndexData()override;
	virtual bool	Frame() override;
	virtual bool	Postrender(ID3D11DeviceContext* pd3dContext);
	virtual void	UpdateIndexBuffer(ID3D11DeviceContext*  pContext, DWORD* pdwIndexArray, int iFaceCount);
	virtual	void	UpdateVertexBuffer(ID3D11DeviceContext* pContext, PNCT_VERTEX* Vertex, int iFaceCount);
public:
	vector<Vector3>				m_FaceNormals;
	vector<SNormalLookupTable>	m_LookupTabel;
public:
	Vector3	ComputeFaceNormal(DWORD i0, DWORD i1, DWORD i2);
	void	CalcFaceNormals();
	void	GetVertexNormal();
	void	InitFaceNormals();
	void	GenNormalLookupTable();
	void	CalcPerVertexNormalsFastLookup();
public:
	SMap();
	virtual ~SMap();
};

