#pragma once
#include <TStd.h>
#include <fbxsdk.h>
#include <unordered_map>
#include <TObject.h>

class SFbxObj
{
	static FbxManager*	g_pSDKManager;
	FbxImporter*		m_pFbxImporter;
	FbxScene*			m_pFBXScene;
	unordered_map<string, Matrix> m_dxMatrixMap;
public:
	bool Load(string	fileName);
	bool LoadFBX(string	fileName);
	bool Initialize(string fileName);
	void PreProcess(FbxNode*	Node);
	void ParseNode(FbxNode* Node, FbxScene* FBXScene);
	void ParseAnimat(FbxScene* FBXScene);
public:
	SFbxObj();
};

