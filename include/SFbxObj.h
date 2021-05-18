#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "SModelObj.h"

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
	for (DWORD i = 0; i < 16; ++i)
	{
		fData[i] = (float)pSrcData[i];
	}
	return matResult;
}

typedef std::unordered_map<FbxNode*, SModelObj*>	sNodeMap;
typedef std::vector<SModelObj*>	sNodeList;
class SFbxObj : public TObject
{
public:
	SScene							m_Scene;
	float							m_fTick = 0;
	static FbxManager*				g_pSDKManager;
	FbxImporter*					m_pFbxImporter;
	FbxScene*						m_pFBXScene;
	unordered_map<string, Matrix>	m_dxMatrixBindPoseMap;
	unordered_map<FbxNode*, int>	m_pFbxNodeMap;
	vector<Matrix>					m_pMatrixList;
	sNodeMap						m_sNodeMap;
	sNodeList						m_sNodeList;
	vector<FbxNode*>				m_pFbxNodeList;
	Vector3	Max = {-9999.0f, -9999.0f, -9999.0f };
	Vector3	Min = { 9999.0f, 9999.0f, 9999.0f };
	Vector2 Boxy = { -9999.0f, 9999.0f };
public:
	ComPtr<ID3D11Buffer>			m_BoneBuffer;
public:
	bool Load(string	fileName);
	bool LoadFBX(string	fileName);
	bool Initialize(string fileName);
	void PreProcess(FbxNode*	Node);
	void ParseNode(FbxNode * Node, Matrix matParent);
	void ParseAnimat(FbxScene* FBXScene);
	Matrix ParseTransform(FbxNode* Node, Matrix& matParent);
	void ParseMesh(FbxNode* Node, FbxMesh* pFbxMesh, SModelObj* obj);
	string ParseMaterial(FbxSurfaceMaterial* pMtrl);
	void ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVset, int vertexIndex, int uvIndex, FbxVector2& uv);
	FbxColor ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* pVertexColorSet, DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4 ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	void ParseAnimStack(FbxScene * FBXScene, FbxString* AnimStackNameArray);
	void ParseNodeAnimation(FbxNode* Node);
	////////
	//bool ParseMeshSkinning(const FbxMesh* pFbxMesh, SkinData* skindata);
	void ParseAnimation(FbxScene*	pFbxScene);
	bool ParseMeshSkinningMap(const FbxMesh* pFbxMesh, vector< SWeight>& skindata);

	bool	CreateInputLayout() override;
public:
	SFbxObj();
};

