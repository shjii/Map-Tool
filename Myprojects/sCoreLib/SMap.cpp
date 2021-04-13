#include "SMap.h"
bool    SMap::CreateVertexData()
{
	// 정점 가로 및 세수 = 2N승+1
	m_VertexList.resize(m_iNumVertices);
	float fHalfCols = (m_iNumCols - 1) / 2.0f;
	float fHalfRows = (m_iNumRows - 1) / 2.0f;
	float fOffsetU = 1.0f / (m_iNumCols - 1);
	float fOffsetV = 1.0f / (m_iNumRows - 1);
	for (int iRow = 0; iRow < m_iNumRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCols; iCol++)
		{
			int iIndex = iRow * m_iNumCols + iCol;
			m_VertexList[iIndex].p.x = (iCol - fHalfCols)*m_fCellDistance;
			m_VertexList[iIndex].p.y = GetHeight(iIndex);
			m_VertexList[iIndex].p.z = (iRow - fHalfRows)*m_fCellDistance*-1.f;
			m_VertexList[iIndex].t.x = iCol * fOffsetU * 1;
			m_VertexList[iIndex].t.y = iRow * fOffsetV * 1;

			m_VertexList[iIndex].n = { 0,1,0 };
			m_VertexList[iIndex].c = { 1,1,1,1 };
		}
	}
	return true;
}
bool    SMap::CreateIndexData()
{
	m_IndexList.resize(m_iNumFaces * 3);
	int iIndex = 0;
	for (int iRow = 0; iRow < m_iNumCellRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCellCols; iCol++)
		{
			int iNextRow = iRow + 1;
			int iNextCol = iCol + 1;
			m_IndexList[iIndex + 0] = iRow * m_iNumCols + iCol;
			m_IndexList[iIndex + 1] = iRow * m_iNumCols + iNextCol;
			m_IndexList[iIndex + 2] = iNextRow * m_iNumCols + iCol;
			m_IndexList[iIndex + 3] = m_IndexList[iIndex + 2];
			m_IndexList[iIndex + 4] = m_IndexList[iIndex + 1];
			m_IndexList[iIndex + 5] = iNextRow * m_iNumCols + iNextCol;
			iIndex += 6;
		}
	}
	m_iNumFaces = m_IndexList.size() / 3;
	GetVertexNormal();
	return true;
}
bool	SMap::Frame()
{

	return true;
}
bool SMap::Postrender(ID3D11DeviceContext * pd3dContext)
{
	pd3dContext->DrawIndexed(m_iNumFaces * 3, 0, 0);
	return true;
}
void SMap::UpdateIndexBuffer(ID3D11DeviceContext * pContext, DWORD * pdwIndexArray, int iFaceCount)
{
	assert(pdwIndexArray);
	m_iNumFaces = iFaceCount;
	pContext->UpdateSubresource(m_pIndexBuffer.Get(), 0, NULL, pdwIndexArray, 0, 0);
}
void SMap::UpdateVertexBuffer(ID3D11DeviceContext * pContext, PNCT_VERTEX* Vertex, int iFaceCount)
{
	assert(Vertex);
	pContext->UpdateSubresource(m_pVertexBuffer.Get(), 0, NULL, Vertex, iFaceCount, 0);
}
float SMap::GetHeight(UINT index)
{
	return 0.0f;
}
float SMap::GetHeightMap(float fPosX, float fPosZ)
{
	float fCellX = (float)(m_iNumCellCols * m_fCellDistance / 2.0f + fPosX);
	float fCellZ = (float)(m_iNumCellRows * m_fCellDistance / 2.0f - fPosZ);

	fCellX /= (float)m_fCellDistance;
	fCellZ /= (float)m_fCellDistance;
	float fVertxCol = ::floorf(fCellX);
	float fVertxRow = ::floorf(fCellZ);
	if (fVertxCol < 0.0f) fVertxCol = 0.0f;
	if (fVertxRow < 0.0f) fVertxRow = 0.0f;
	if ((float)(m_iNumCols - 2) < fVertxCol) fVertxCol = (float)(m_iNumCols - 2);
	if ((float)(m_iNumRows - 2) < fVertxRow) fVertxRow = (float)(m_iNumRows - 2);
	float A = GetHeightMap((int)fVertxRow, (int)fVertxCol);
	float B = GetHeightMap((int)fVertxRow, (int)fVertxCol+1);
	float C = GetHeightMap((int)fVertxRow+1, (int)fVertxCol);
	float D = GetHeightMap((int)fVertxRow+1, (int)fVertxCol+1);

	float fDeltaX = fCellX - fVertxCol;
	float fDellaZ = fCellZ - fVertxRow;
	float fHeight = 0.0f;
	if (fDellaZ < (1.0f - fDeltaX))
	{
		float uy = B - A;
		float vy = C - A;
		fHeight = A + Lerp(0.0f, uy, fDeltaX) + Lerp(0.0f, vy, fDellaZ);
	}
	else
	{
		float uy = C - D;
		float vy = B - D;
		fHeight = D + Lerp(0.0f, uy, 1.0f - fDeltaX) + Lerp(0.0f, vy, 1.0f - fDellaZ);
	}
	return  fHeight;
}
float SMap::GetHeightMap(int row, int col)
{
	return m_VertexList[row * m_iNumRows + col].p.y;
}
float SMap::Lerp(float fStart, float fEnd, float fTangent)
{
	return fStart - (fStart * fTangent) + (fEnd * fTangent);
}
bool SMap::CreateMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, SMapDesc  desc)
{
	m_MapDesc = desc;
	m_iNumRows = desc.iNumRows;
	m_iNumCols = desc.iNumCols;
	m_iNumCellCols = m_iNumCols - 1;
	m_iNumCellRows = m_iNumRows - 1;
	m_iNumVertices = m_iNumCols * m_iNumRows;
	m_iNumFaces = m_iNumCellCols * m_iNumCellRows * 2;
	m_fCellDistance = desc.fCellDistance;

	Create(pDevice,
		desc.szVS,
		desc.szPS,
		desc.szTexFile);
	return true;
}
SMap::SMap()
{

}
SMap::~SMap()
{

}

///////////////////

Vector3	SMap::ComputeFaceNormal(DWORD i0, DWORD i1, DWORD i2)
{
	Vector3 normal;
	Vector3 v0 = m_VertexList[i1].p - m_VertexList[i0].p;
	Vector3 v1 = m_VertexList[i2].p - m_VertexList[i0].p;
	normal = v0.Cross(v1);
	normal.Normalize();
	return normal;
}
void	SMap::CalcFaceNormals()
{
	int index = 0;
	for (int i = 0; i < m_iNumFaces * 3; i+=3)
	{
		m_FaceNormals[index++] = ComputeFaceNormal(m_IndexList[i], m_IndexList[i+1], m_IndexList[i+2]);
	}
}
void	SMap::GetVertexNormal()
{
	InitFaceNormals();
	GenNormalLookupTable();
	CalcPerVertexNormalsFastLookup();
}
void	SMap::InitFaceNormals()
{
	m_FaceNormals.resize(m_iNumFaces);
	for (int i = 0; i < m_iNumFaces; i++)
	{
		m_FaceNormals[i] = Vector3::Zero;
	}
}
void	SMap::GenNormalLookupTable()
{
	m_LookupTabel.resize(m_iNumVertices);
	for (int i = 0; i < m_iNumFaces; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 6; k++)
			{
				int id = m_IndexList[i * 3 + j];
				if (m_LookupTabel[id].index[k] == -1)
				{
					m_LookupTabel[id].index[k] = i;
					break;
				}
			}
		}
	}
}
void	SMap::CalcPerVertexNormalsFastLookup()
{
	CalcFaceNormals();
	for (int i = 0; i < m_iNumVertices; i++)
	{
		Vector3 avgNormal = Vector3::Zero;
		for (int j = 0; j < 6; j++)
		{
			int index = m_LookupTabel[i].index[j];
			if (index != -1)
			{
				avgNormal += m_FaceNormals[index];
			}
			else
			{
				break;
			}
		}
		m_VertexList[i].n = avgNormal;
		m_VertexList[i].n.Normalize();
	}
}