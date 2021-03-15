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
	ParseNode(RootNode, m_pFBXScene);
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

void SFbxObj::ParseNode(FbxNode * Node, FbxScene * FBXScene)
{
	shared_ptr<TObject> obj = make_shared<TObject>();
	obj->m_szName = to_mw(Node->GetName());
}

void SFbxObj::ParseAnimat(FbxScene * FBXScene)
{
}

SFbxObj::SFbxObj()
{
	m_pFbxImporter = nullptr;
	m_pFBXScene = nullptr;
}
