#pragma once
#include <TStd.h>
#include <fbxsdk.h>
#include <unordered_map>
#include <TObject.h>
typedef std::unordered_map<FbxNode*, TObject*>	sMeshMap;
class SFbxObj
{
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
	void ParseMesh(FbxNode* Node, FbxMesh* pFbxMesh, TObject* obj);
	string ParseMaterial(FbxSurfaceMaterial* pMtrl);
public:
	SFbxObj();
};

