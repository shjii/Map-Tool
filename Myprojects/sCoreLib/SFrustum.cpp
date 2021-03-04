#include "SFrustum.h"
BOOL SFrustum::CheckOBBInPlane(S_BOX*  pBox)
{
	float		fPlaneToCenter = 0.0;
	float		distance = 0.0f;
	Vector3 vDir;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = pBox->vAxis[0] * pBox->fExtent[0];
		distance = fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b*vDir.y + m_Plane[iPlane].c * vDir.z);
		vDir = pBox->vAxis[1] * pBox->fExtent[1];
		distance += fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b*vDir.y + m_Plane[iPlane].c * vDir.z);
		vDir = pBox->vAxis[2] * pBox->fExtent[2];
		distance += fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b*vDir.y + m_Plane[iPlane].c * vDir.z);

		fPlaneToCenter = m_Plane[iPlane].a * pBox->vCenter.x + m_Plane[iPlane].b*pBox->vCenter.y +
			m_Plane[iPlane].c * pBox->vCenter.z + m_Plane[iPlane].d;

		if (fPlaneToCenter <= -distance)
		{
			return FALSE;
		}
	}
	return TRUE;
}
S_POSITION SFrustum::CheckPoitionOBBInPlane(S_BOX * pBox)
{
	// 1. 모두 포함하면  =1
	// 2. 겹치면         =2
	// 3. 밖에있으면     =0


	float		fPlaneToCenter = 0.0;
	float		distance = 0.0f;
	Vector3 vDir;
	S_POSITION  s_Position;

	s_Position = P_FRONT;

	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = pBox->vAxis[0] * pBox->fExtent[0];
		distance = fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b*vDir.y + m_Plane[iPlane].c * vDir.z);
		vDir = pBox->vAxis[1] * pBox->fExtent[1];
		distance += fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b*vDir.y + m_Plane[iPlane].c * vDir.z);
		vDir = pBox->vAxis[2] * pBox->fExtent[2];
		distance += fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b*vDir.y + m_Plane[iPlane].c * vDir.z);

		fPlaneToCenter = m_Plane[iPlane].a * pBox->vCenter.x + m_Plane[iPlane].b*pBox->vCenter.y +
			m_Plane[iPlane].c * pBox->vCenter.z + m_Plane[iPlane].d;

		if (fPlaneToCenter <= distance)
		{
			s_Position = P_SPANNING;
		}

		if (fPlaneToCenter + 1.0f < -distance)
		{
			return P_BACK;
		}
	}

	return s_Position;
}
bool SFrustum::Create(ID3D11Device * pd3dDevice)
{
	if (!m_FrustumObj.Create(pd3dDevice, L"vs.txt", L"ps.txt", L""))
	{
		return false;
	}

	for (auto v : m_FrustumObj.m_VertexList)
	{
		m_VertexList.push_back(v);
	}
	m_VertexList[0].p = Vector3(-1.0f, 1.0f, 0.0f);
	m_VertexList[1].p = Vector3(1.0f, 1.0f, 0.0f);
	m_VertexList[2].p = Vector3(1.0f, -1.0f, 0.0f);
	m_VertexList[3].p = Vector3(-1.0f, -1.0f, 0.0f);

	m_VertexList[8].p = Vector3(1.0f, 1.0f, 0.0f);
	m_VertexList[11].p = Vector3(1.0f, -1.0f, 0.0f);

	m_VertexList[13].p = Vector3(-1.0f, 1.0f, 0.0f);
	m_VertexList[14].p = Vector3(-1.0f, -1.0f, 0.0f);

	m_VertexList[18].p = Vector3(1.0f, 1.0f, 0.0f);
	m_VertexList[19].p = Vector3(-1.0f, 1.0f, 0.0f);


	m_VertexList[20].p = Vector3(-1.0f, -1.0f, 0.0f);
	m_VertexList[21].p = Vector3(1.0f, -1.0f, 0.0f);

	m_Plane.resize(6);
	return true;
}

bool SFrustum::Frame()
{
	//if (m_Plane.size() <= 0) return true;
	//m_Plane[0].Create(
	//	m_FrustumObj.m_VertexList[2].p,
	//	m_FrustumObj.m_VertexList[1].p,
	//	m_FrustumObj.m_VertexList[0].p);
	//m_Plane[1].Create(
	//	m_FrustumObj.m_VertexList[6].p,
	//	m_FrustumObj.m_VertexList[5].p,
	//	m_FrustumObj.m_VertexList[4].p);
	//m_Plane[2].Create(
	//	m_FrustumObj.m_VertexList[10].p,
	//	m_FrustumObj.m_VertexList[9].p,
	//	m_FrustumObj.m_VertexList[8].p);
	//m_Plane[3].Create(
	//	m_FrustumObj.m_VertexList[14].p,
	//	m_FrustumObj.m_VertexList[13].p,
	//	m_FrustumObj.m_VertexList[12].p);
	//m_Plane[4].Create(
	//	m_FrustumObj.m_VertexList[18].p,
	//	m_FrustumObj.m_VertexList[17].p,
	//	m_FrustumObj.m_VertexList[16].p);
	//m_Plane[5].Create(
	//	m_FrustumObj.m_VertexList[22].p,
	//	m_FrustumObj.m_VertexList[21].p,
	//	m_FrustumObj.m_VertexList[20].p);
	return true;
}

BOOL SFrustum::ClassifyPoint(Vector3 v)
{
	for (int iPlane = 0; iPlane < m_Plane.size(); iPlane++)
	{
		float dist = m_Plane[iPlane].a * v.x + m_Plane[iPlane].b * v.y + m_Plane[iPlane].c * v.z + m_Plane[iPlane].d;
		if (dist < 0.0f)
		{
			return false;
		}
	}
	return true;
}

BOOL SFrustum::ClassifyBox(SShapeBox box)
{
	return 0;
}

bool SFrustum::CreateFrustum()
{
	Matrix m_matViewProj = m_mView * m_mProj;
	ExtractPlanesD3D(m_Plane, m_matViewProj);


	m_matViewProj.Invert();

	m_vFrustum[0] = Vector3(-1.0f, -1.0f, 0.0f);
	m_vFrustum[1] = Vector3(-1.0f, 1.0f, 0.0f);
	m_vFrustum[2] = Vector3(1.0f, 1.0f, 0.0f);
	m_vFrustum[3] = Vector3(1.0f, -1.0f, 0.0f);
					
	m_vFrustum[4] = Vector3(-1.0f, -1.0f, 1.0f);
	m_vFrustum[5] = Vector3(-1.0f, 1.0f, 1.0f);
	m_vFrustum[6] = Vector3(1.0f, 1.0f, 1.0f);
	m_vFrustum[7] = Vector3(1.0f, -1.0f, 1.0f);

	for (int iVertex = 0; iVertex < 8; iVertex++)
	{
		m_vFrustum[iVertex] = Vector3::Transform(m_vFrustum[iVertex], m_matViewProj);
		//m_vFrustum[iVertex].Transform(&m_matViewProj);
	}
	m_Plane.resize(6);
	// 노말 방향 안쪽으로
	m_Plane[0]=	CreatePlane(m_vFrustum[5], m_vFrustum[0], m_vFrustum[1]);	
	m_Plane[1]=	CreatePlane(m_vFrustum[3], m_vFrustum[6], m_vFrustum[2]);	
	m_Plane[2]=	CreatePlane(m_vFrustum[5], m_vFrustum[2], m_vFrustum[6]);	
	m_Plane[3]=	CreatePlane(m_vFrustum[0], m_vFrustum[7], m_vFrustum[3]);	
	m_Plane[4]=	CreatePlane(m_vFrustum[0], m_vFrustum[2], m_vFrustum[1]);	
	m_Plane[5]=	CreatePlane(m_vFrustum[6], m_vFrustum[4], m_vFrustum[5]);	
	return true;
}
SPlane SFrustum::CreatePlane(Vector3 v0, Vector3 v1, Vector3 v2)
{
	Vector3 vEdge0 = v1 - v0;
	Vector3 vEdge1 = v2 - v0;
	Vector3 vNormal;
	vNormal = vEdge0.Cross(vEdge1);
	vNormal.Normalize();
	SPlane s;

	s.a = vNormal.x;	s.b = vNormal.y;	s.c = vNormal.z;
	s.d = -(s.a*v0.x + s.b * v0.y + s.c * v0.z);
	return s;
}
void SFrustum::SetMatrix(Matrix* pWorld, Matrix* pView, Matrix* pProj)
{
	if (pWorld != NULL)
	{
		m_mWorld = *pWorld;
	}
	else
	{
		m_mWorld = ::Matrix::Identity;
	}
	if (pView != NULL)
	{
		m_mView = *pView;
	}
	if (pProj != NULL)
	{
		m_mProj = *pProj;
	}
}

void SFrustum::ExtractPlanesD3D(vector<SPlane>& pPlanes, const Matrix& comboMatrix)
{
	if (pPlanes.size() == 0) return;

	// Left clipping plane
	pPlanes[0].a = comboMatrix._14 + comboMatrix._11;
	pPlanes[0].b = comboMatrix._24 + comboMatrix._21;
	pPlanes[0].c = comboMatrix._34 + comboMatrix._31;
	pPlanes[0].d = comboMatrix._44 + comboMatrix._41;
	// Right clipping plane
	pPlanes[1].a = comboMatrix._14 - comboMatrix._11;
	pPlanes[1].b = comboMatrix._24 - comboMatrix._21;
	pPlanes[1].c = comboMatrix._34 - comboMatrix._31;
	pPlanes[1].d = comboMatrix._44 - comboMatrix._41;
	// Top clipping plane
	pPlanes[2].a = comboMatrix._14 - comboMatrix._12;
	pPlanes[2].b = comboMatrix._24 - comboMatrix._22;
	pPlanes[2].c = comboMatrix._34 - comboMatrix._32;
	pPlanes[2].d = comboMatrix._44 - comboMatrix._42;
	// Bottom clipping plane
	pPlanes[3].a = comboMatrix._14 + comboMatrix._12;
	pPlanes[3].b = comboMatrix._24 + comboMatrix._22;
	pPlanes[3].c = comboMatrix._34 + comboMatrix._32;
	pPlanes[3].d = comboMatrix._44 + comboMatrix._42;
	// Near clipping plane
	pPlanes[4].a = comboMatrix._13;
	pPlanes[4].b = comboMatrix._23;
	pPlanes[4].c = comboMatrix._33;
	pPlanes[4].d = comboMatrix._43;
	// Far clipping plane
	pPlanes[5].a = comboMatrix._14 - comboMatrix._13;
	pPlanes[5].b = comboMatrix._24 - comboMatrix._23;
	pPlanes[5].c = comboMatrix._34 - comboMatrix._33;
	pPlanes[5].d = comboMatrix._44 - comboMatrix._43;

	pPlanes[0].Normalize();
	pPlanes[1].Normalize();
	pPlanes[2].Normalize();
	pPlanes[3].Normalize();
	pPlanes[4].Normalize();
	pPlanes[5].Normalize();
}