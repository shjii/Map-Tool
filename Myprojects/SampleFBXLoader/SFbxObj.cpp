#include "SFbxObj.h"
FbxManager* SFbxObj::g_pSDKManager = nullptr;

bool SFbxObj::Load(string fileName)
{
	if (LoadFBX(fileName))
	{
		return true;
	}
	return false;
}

bool SFbxObj::LoadFBX(string fileName)
{
	if (Initialize(fileName) == false)
	{
		return false;
	}
	FbxNode* RootNode = m_pFBXScene->GetRootNode();
	PreProcess(RootNode);
	ParseNode(RootNode, Matrix::Identity);
	ParseAnimat(m_pFBXScene);
	return true;
}

bool SFbxObj::Initialize(string fileName)
{
	if (g_pSDKManager == nullptr)
	{
		g_pSDKManager = FbxManager::Create();
		if (g_pSDKManager == nullptr) return false;
		FbxIOSettings* ios = FbxIOSettings::Create(g_pSDKManager, IOSROOT);
		if (ios = nullptr) return false;
		g_pSDKManager->SetIOSettings(ios);
	}
	if (m_pFbxImporter == nullptr)
	{
		m_pFbxImporter = FbxImporter::Create(g_pSDKManager, "");
		if (m_pFbxImporter == nullptr) return false;
	}
	if (m_pFBXScene == nullptr)
	{
		m_pFBXScene = FbxScene::Create(g_pSDKManager, "");
		if (m_pFBXScene == nullptr) return false;
	}
	bool bRet = m_pFbxImporter->Initialize(fileName.c_str(), -1, g_pSDKManager->GetIOSettings());
	if (bRet == false) return false;
	bRet = m_pFbxImporter->Import(m_pFBXScene);

	FbxGeometryConverter IGomConverter(g_pSDKManager);
	IGomConverter.Triangulate(m_pFBXScene, true);
	return true;
}

void SFbxObj::PreProcess(FbxNode * Node)
{
	if (Node && (Node->GetCamera() || Node->GetLight()))
	{
		return;
	}
	Matrix mat = mat.Identity;
	auto iter = m_dxMatrixMap.find(Node->GetName());
	if (iter == m_dxMatrixMap.end())
	{
		m_dxMatrixMap[Node->GetName()] = mat;
	}
	int dwChild = Node->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = Node->GetChild(dwObj);
		if (pChildNode->GetNodeAttribute() != NULL)
		{
			FbxNodeAttribute::EType AtteribteType = pChildNode->GetNodeAttribute()->GetAttributeType();
			if (AtteribteType != FbxNodeAttribute::eMesh &&
				AtteribteType != FbxNodeAttribute::eSkeleton&&
				AtteribteType != FbxNodeAttribute::eNull)
			{
				continue;
			}
		}
		PreProcess(pChildNode);
	}
}

Matrix SFbxObj::ParesTransform(FbxNode* Node, Matrix& matParent)
{
	Matrix matWorld = Matrix::Identity;
	return matWorld;
}

void SFbxObj::ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVset, int vertexIndex, int uvIndex, FbxVector2& uv)
{
	FbxLayerElementUV *pFbxLayerElementUV = pUVset;

	if (pFbxLayerElementUV == nullptr) {
		return;
	}

	switch (pFbxLayerElementUV->GetMappingMode())
	{
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (pFbxLayerElementUV->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		{
			FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(vertexIndex);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		case FbxLayerElementUV::eIndexToDirect:
		{
			int id = pFbxLayerElementUV->GetIndexArray().GetAt(vertexIndex);
			FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(id);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		}
		break;
	}
	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (pFbxLayerElementUV->GetReferenceMode())
		{
			// Always enters this part for the example model
		case FbxLayerElementUV::eDirect:
		case FbxLayerElementUV::eIndexToDirect:
		{
			uv.mData[0] = pFbxLayerElementUV->GetDirectArray().GetAt(uvIndex).mData[0];
			uv.mData[1] = pFbxLayerElementUV->GetDirectArray().GetAt(uvIndex).mData[1];
			break;
		}
		}
		break;
	}
	}
}

void SFbxObj::ParseMesh(FbxNode* Node, FbxMesh* pFbxMesh, TObject* obj)
{
	vector<FbxLayerElementUV*> VertexUVSets;
	int iLayerCount = pFbxMesh->GetLayerCount();
	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	{
		FbxLayer* pLayer = pFbxMesh->GetLayer(iLayer);
		if (pLayer->GetVertexColors() != NULL)
		{

		}
		if (pLayer->GetUVs() != NULL)
		{
			VertexUVSets.push_back(pLayer->GetUVs());
		}
	}
	vector<string> fbxMaterialList;
	int iNumMtrl = Node->GetMaterialCount();
	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		FbxSurfaceMaterial* pMtrl = Node->GetMaterial(iMtrl);
		if (pMtrl == nullptr)
		{
			continue;
		}
		fbxMaterialList.push_back(ParseMaterial(pMtrl));
	}

	int iPolyCount = pFbxMesh->GetPolygonCount();
	int iTriangleCount = pFbxMesh->GetControlPointsCount();
	FbxVector4* pVertexPosiions = pFbxMesh->GetControlPoints();

	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
	{
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
		int iTriangleCount = iPolySize - 2;

		int iCornerIndices[3];
		STri tri;
		for (int iTriangle = 0; iTriangle < iTriangleCount; iTriangle++)
		{
			INT iVertIndex[3] = { 0, (INT)iTriangle + 2, (INT)iTriangle + 1 };
			iCornerIndices[0] = pFbxMesh->GetPolygonVertex(iPoly, iVertIndex[0]);
			iCornerIndices[1] = pFbxMesh->GetPolygonVertex(iPoly, iVertIndex[1]);
			iCornerIndices[2] = pFbxMesh->GetPolygonVertex(iPoly, iVertIndex[2]);

			FbxVector4 vNormals[3];
			ZeroMemory(vNormals, 3* sizeof(FbxVector4));
			INT iPolyIndex = (INT)iPoly;
			pFbxMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[0], vNormals[0]);
			pFbxMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[1], vNormals[1]);
			pFbxMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[2], vNormals[2]);

			int u[3];
			u[0] = pFbxMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[0]);
			u[1] = pFbxMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[1]);
			u[2] = pFbxMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[2]);

			for (int iIndex = 0; iIndex < 3; iIndex++)
			{
				PNCT_VERTEX v;
				v.p.x = pVertexPosiions[iCornerIndices[iIndex]].mData[0];
				v.p.y = pVertexPosiions[iCornerIndices[iIndex]].mData[2];
				v.p.z = pVertexPosiions[iCornerIndices[iIndex]].mData[1];
				v.c = Vector4(1,1,1,1);
				v.n.x = vNormals[iCornerIndices[iIndex]].mData[0];
				v.n.y = vNormals[iCornerIndices[iIndex]].mData[2];
				v.n.z = vNormals[iCornerIndices[iIndex]].mData[1];
				for (int iUVIndex = 0; iUVIndex < VertexUVSets.size(); iUVIndex++)
				{
					FbxLayerElementUV* pUVSet = VertexUVSets[iUVIndex];
					FbxVector2 uv(0,0);
					ReadTextureCoord(pFbxMesh, pUVSet, iCornerIndices[iIndex],u[iIndex],uv);
					v.t.x = uv.mData[0];
					v.t.y = 1.0f - uv.mData[1];
				}
				tri.vVertex[iIndex] = v;
			}
			obj->m_TriangleList.push_back(tri);
		}
	}
}

string SFbxObj::ParseMaterial(FbxSurfaceMaterial* pMtrl)
{
	string name = pMtrl->GetName();
	auto Property = pMtrl->FindProperty(FbxSurfaceMaterial::sDiffuse);
	if (Property.IsValid())
	{
		const FbxFileTexture* tex = Property.GetSrcObject<FbxFileTexture>();
		if (tex != nullptr)
		{
			return tex->GetFileName();
		}
	}
	return "";
}

void SFbxObj::ParseNode(FbxNode * Node, Matrix matParent)
{
	if (Node == nullptr) return;
	if (Node && (Node->GetCamera() || Node->GetLight()))
	{
		return;
	}
	TObject* obj = new TObject;
	obj->m_szName = to_mw(Node->GetName());
	
	m_sMeshMap[Node] = obj;
	Matrix matWorld = ParesTransform(Node, matParent);
	obj->m_matWorld = matWorld;
	if (Node->GetMesh() != nullptr)
	{
		ParseMesh(Node, Node->GetMesh(), obj);
	}
	int dwChild = Node->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = Node->GetChild(dwObj);
		ParseNode(pChildNode, matWorld);
	}
}

void SFbxObj::ParseAnimat(FbxScene * FBXScene)
{
}

SFbxObj::SFbxObj()
{
	m_pFbxImporter = nullptr;
	m_pFBXScene = nullptr;
}
