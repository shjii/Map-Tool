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

	FbxAxisSystem::MayaZUp.ConvertScene(m_pFBXScene);
	FbxAxisSystem SceneAxisSystem = m_pFBXScene->GetGlobalSettings().GetAxisSystem();
	//FbxGeometryConverter IGomConverter(g_pSDKManager);
	//IGomConverter.Triangulate(m_pFBXScene, true);
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
		m_pNodeMap[Node] = m_pMatrixList.size();
		m_pMatrixList.push_back(mat);
	}
	int dwChild = Node->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = Node->GetChild(dwObj);
		//if (pChildNode->GetNodeAttribute() != NULL)
		//{
		//	FbxNodeAttribute::EType AtteribteType = pChildNode->GetNodeAttribute()->GetAttributeType();
		//	if (AtteribteType != FbxNodeAttribute::eMesh &&
		//		AtteribteType != FbxNodeAttribute::eSkeleton&&
		//		AtteribteType != FbxNodeAttribute::eNull)
		//	{
		//		continue;
		//	}
		//}
		PreProcess(pChildNode);
	}
}

Matrix SFbxObj::ParesTransform(FbxNode* Node, Matrix& matParent)
{
	Matrix matWorld = Matrix::Identity;
	return matWorld;
}

void SFbxObj::ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex, FbxVector2& uv) {

	FbxLayerElementUV *pFbxLayerElementUV = pUVSet;

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


void SFbxObj::ParseMesh(FbxNode* Node, FbxMesh* pFbxMesh, SModelObj* obj)
{
	vector<FbxLayerElementUV*> VertexUVSets;
	vector<FbxLayerElementMaterial*> pMaterialSetList;
	vector<FbxLayerElementVertexColor*> VertexColorSet;
	int iLayerCount = pFbxMesh->GetLayerCount();
	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	{
		FbxLayer* pLayer = pFbxMesh->GetLayer(iLayer);
		if (pLayer->GetVertexColors() != NULL)
		{
			VertexColorSet.push_back(pLayer->GetVertexColors());
		}
		if (pLayer->GetUVs() != NULL)
		{
			VertexUVSets.push_back(pLayer->GetUVs());
		}
		if (pFbxMesh->GetLayer(iLayer)->GetMaterials() != nullptr)
		{
			pMaterialSetList.push_back(pFbxMesh->GetLayer(iLayer)->GetMaterials());
		}
	}
	
	int iNumMtrl = Node->GetMaterialCount();
	if(iNumMtrl > 1)obj->m_subMesh.resize(iNumMtrl);
	else { obj->m_subMesh.resize(1); }
	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		FbxSurfaceMaterial* pMtrl = Node->GetMaterial(iMtrl);
		if (pMtrl == nullptr)
		{
			continue;
		}
		obj->fbxMaterialList.push_back(to_mw(ParseMaterial(pMtrl)));
	}

	bool bSkinnedMesh = ParseMeshSkinningMap(pFbxMesh, obj->WeightList);
	obj->m_bSkinnedMesh = bSkinnedMesh;


	FbxAMatrix geom;
	FbxVector4 trans = Node->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rot = Node->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scale = Node->GetGeometricScaling(FbxNode::eSourcePivot);
	geom.SetT(trans);
	geom.SetR(rot);
	geom.SetS(scale);

	FbxAMatrix normalMatrix = geom;
	normalMatrix = normalMatrix.Inverse();
	normalMatrix = normalMatrix.Transpose();

	obj->m_matWorld = DxConvertMatrix(ConvertMatrixA(Node->EvaluateGlobalTransform(1.0f)));

	int iPolyCount = pFbxMesh->GetPolygonCount();
	int iTriangleCount = pFbxMesh->GetControlPointsCount();
	FbxVector4* pVertexPosiions = pFbxMesh->GetControlPoints();

	int	iBasePolyIndex = 0;
	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
	{
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
		int iTriangleCount = iPolySize - 2;

		int iCornerIndices[3];
		STri tri;

		int iSubMtrl = 0;
		if (pMaterialSetList.size() > 0)
		{
			switch (pMaterialSetList[0]->GetMappingMode())
			{
			case FbxLayerElement::eByPolygon:
			{
				switch (pMaterialSetList[0]->GetReferenceMode())
				{
				case FbxLayerElement::eIndex:
				{
					iSubMtrl = iPoly;
				}break;
				case FbxLayerElement::eIndexToDirect:
				{
					iSubMtrl = pMaterialSetList[0]->GetIndexArray().GetAt(iPoly);
				}break;
				}
			}
		}
	}


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
				auto fina = geom.MultT(pVertexPosiions[iCornerIndices[iIndex]]);
				v.p.x = fina.mData[0];
				v.p.y = fina.mData[2];
				v.p.z = fina.mData[1];
				FbxColor color;
				if (VertexColorSet.empty())
				{
					color.Set(1, 1, 1, 1);
				}
				else
				{
					color = ReadColor(pFbxMesh, VertexColorSet.size(), VertexColorSet[0], iCornerIndices[iIndex], iBasePolyIndex + iVertIndex[iIndex]);
				}

				v.c.x = (float)color.mRed;
				v.c.y = (float)color.mGreen;
				v.c.z = (float)color.mBlue;
				v.c.w = 1;


				FbxVector4 normal = ReadNormal(pFbxMesh, iCornerIndices[iIndex], iBasePolyIndex + iVertIndex[iIndex]);
				fina = normalMatrix.MultT(normal);

				v.n.x = fina.mData[0]; // x
				v.n.y = fina.mData[2]; // z
				v.n.z = fina.mData[1]; // y
				if (VertexUVSets.size())
				{
					for (int iUVIndex = 0; iUVIndex < 1/*VertexUVSets.size()*/; iUVIndex++)
					{
						FbxLayerElementUV* pUVSet = VertexUVSets[iUVIndex];
						FbxVector2 uv(0, 0);
						ReadTextureCoord(pFbxMesh, pUVSet, iCornerIndices[iIndex], u[iIndex], uv);
						v.t.x = uv.mData[0];
						v.t.y = 1.0f - uv.mData[1];
					}
				}

				IW_VERTEX iw;
				if (obj->m_bSkinnedMesh)
				{
					SWeight* pW = &obj->WeightList[iCornerIndices[iIndex]];
					for (int i = 0; i < 4; i++)
					{
						iw.i[i] = pW->Index[i];
						iw.w[i] = pW->Weight[i];
					}
				}
				else
				{
					auto data = m_pNodeMap.find(Node);
					iw.i[0] = data->second;
					iw.w[0] = 1.0f;
				}
				tri.vVertex[iIndex] = v;
				tri.vVertexIW[iIndex] = iw;
			}

			if (iNumMtrl < 1)
			{
				obj->m_subMesh[0].m_TriangleList.push_back(tri);
			}
			else
			{
				obj->m_subMesh[iSubMtrl].m_TriangleList.push_back(tri);
			}	
		}
		iBasePolyIndex += iPolySize;
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
			const CHAR* szFileName = tex->GetFileName();
			CHAR Drive[MAX_PATH];
			CHAR Dir[MAX_PATH];
			CHAR FName[MAX_PATH];
			CHAR Ext[MAX_PATH];
			_splitpath_s(szFileName, Drive, Dir, FName, Ext);
			std::string texName = FName;
			string a = Ext;
			if (a == ".tga" || a == ".TGA")
			{
				a = ".dds";
			}
			texName += a;
			return texName;
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
	SModelObj* obj = new SModelObj;
	obj->m_szName = to_mw(Node->GetName());
	
	m_sNodeList.push_back(obj);
	m_sNodeMap[Node] = obj;
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
SFbxObj::SFbxObj()
{
	m_pFbxImporter = nullptr;
	m_pFBXScene = nullptr;
}

FbxColor SFbxObj::ReadColor(const FbxMesh* mesh,DWORD dwVertexColorCount,FbxLayerElementVertexColor* pVertexColorSet,DWORD dwDCCIndex, DWORD dwVertexIndex)
{
	FbxColor Value(1, 1, 1, 1);
	if (dwVertexColorCount > 0 && pVertexColorSet != NULL)
	{
		// Crack apart the FBX dereferencing system for Color coordinates		
		switch (pVertexColorSet->GetMappingMode())
		{
		case FbxLayerElement::eByControlPoint:
			switch (pVertexColorSet->GetReferenceMode())
			{
			case FbxLayerElement::eDirect:
			{
				Value = pVertexColorSet->GetDirectArray().GetAt(dwDCCIndex);
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwDCCIndex);
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}break;
			}
		case FbxLayerElement::eByPolygonVertex:
			switch (pVertexColorSet->GetReferenceMode())
			{
			case FbxLayerElement::eDirect:
			{
				int iColorIndex = dwVertexIndex;
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwVertexIndex);
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}break;
			}
			break;
		}
	}
	return Value;
}

FbxVector4 SFbxObj::ReadNormal(const FbxMesh* mesh,	int controlPointIndex,	int vertexCounter)
{
	if (mesh->GetElementNormalCount() < 1) {}

	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	// 노말 획득 
	FbxVector4 result;
	// 노말 벡터를 저장할 벡터 
	switch (vertexNormal->GetMappingMode()) 	// 매핑 모드 
	{
		// 제어점 마다 1개의 매핑 좌표가 있다.
	case FbxGeometryElement::eByControlPoint:
	{
		// control point mapping 
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		} break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
			// 인덱스를 얻어온다. 
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}break;
	}break;
	// 정점 마다 1개의 매핑 좌표가 있다.
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
			// 인덱스를 얻어온다. 
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}
	}break;
	}
	return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

int SkinData::iNumMaxWeight = 0;
void	SkinData::Alloc(size_t dwCount, DWORD dwStride)
{
	dwVertexCount = dwCount;
	dwVertexStride = dwStride;

	size_t dwBufferSize = dwVertexCount * dwVertexStride;
	pBoneIndices.reset(new int[dwBufferSize]);
	ZeroMemory(pBoneIndices.get(), sizeof(float) * dwBufferSize);

	pBoneWeights.reset(new float[dwBufferSize]);
	ZeroMemory(pBoneWeights.get(), sizeof(float) * dwBufferSize);
}
int*	SkinData::GetIndices(size_t dwIndex)
{
	assert(dwIndex < dwVertexCount);
	return pBoneIndices.get() + (dwIndex * dwVertexStride);
}
float*	SkinData::GetWeights(size_t dwIndex)
{
	assert(dwIndex < dwVertexCount);
	return pBoneWeights.get() + (dwIndex * dwVertexStride);
}
DWORD	SkinData::GetBoneCount() const
{
	return static_cast<DWORD>(InfluenceNodes.size());
}
void	SkinData::InsertWeight(size_t dwIndex, DWORD dwBoneIndex, float fBoneWeight)
{
	assert(dwBoneIndex < 256);

	auto pIndices = GetIndices(dwIndex);
	auto pWeights = GetWeights(dwIndex);

	for (DWORD i = 0; i < dwVertexStride; i++)
	{
		if (fBoneWeight > pWeights[i])
		{
			for (DWORD j = (dwVertexStride - 1); j < i; --j)
			{
				pIndices[j] = pIndices[j - 1];
				pWeights[j] = pWeights[j - 1];
			}
			pIndices[i] = static_cast<int>(dwBoneIndex);
			pWeights[i] = fBoneWeight;
			break;
		}
		if (iNumMaxWeight < i)
		{
			iNumMaxWeight = i;
		}
	}
}