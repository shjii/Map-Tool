#pragma once
#include <TStd.h>
#include <fbxsdk.h>
#include <unordered_map>
#include <TObject.h>


static Matrix DxConvertMatrix(Matrix m)
{
	Matrix mat;
	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;

	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._44 = 1.0f;
	return mat;
}
static Matrix ConvertMatrixA(const FbxMatrix& matrix)
{
	Matrix matResult;
	auto fData = reinterpret_cast<float*>(&matResult);
	auto pSrcData = reinterpret_cast<const DOUBLE*>(&matrix);
	for (DWORD i = 0; i < 16; i++)
	{
		fData[i] = (float)pSrcData[i];
	}
	return matResult;
}
class SModelObject : public TObject
{
public:
	vector<wstring> fbxMaterialList;
};
typedef std::unordered_map<FbxNode*, SModelObject*>	sMeshMap;
class SFbxObj
{
public:
	static FbxManager*	g_pSDKManager;
	FbxImporter*		m_pFbxImporter;
	FbxScene*			m_pFBXScene;
	unordered_map<string, Matrix> m_dxMatrixMap;
	sMeshMap			m_sMeshMap;
public:
	bool Load(string	fileName);
	bool LoadFBX(string	fileName);
	bool Initialize(string fileName);
	void PreProcess(FbxNode*	Node);
	void ParseNode(FbxNode * Node, Matrix matParent);
	void ParseAnimat(FbxScene* FBXScene);
	Matrix ParesTransform(FbxNode* Node, Matrix& matParent);
	void ParseMesh(FbxNode* Node, FbxMesh* pFbxMesh, SModelObject* obj);
	string ParseMaterial(FbxSurfaceMaterial* pMtrl);
	void ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVset, int vertexIndex, int uvIndex, FbxVector2& uv);
public:
	SFbxObj();
};

