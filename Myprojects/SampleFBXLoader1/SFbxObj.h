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
struct subMesh
{
	std::vector<STri>			m_TriangleList;
	std::vector<PNCT_VERTEX>	m_VertexList;
	ComPtr<ID3D11Buffer>		m_pVertexBuffer;
	STexture*					m_pTexture;
	int							m_count;
	~subMesh()
	{

	}
};
struct SWeight
{
	vector<int> Index;
	vector<float> Weight;
};
struct SScene
{
	int iFirstFrame;
	int iLastFrame;
	int iFrameSpeed; // 30
	int iTickPerFrame;// 160
	int iNumMesh;
	int iDeltaTick; // 1frame
	float fDeltaTime;
	float fFirstTime;
	float fLastTime;
};
struct SAnimTrack
{
	int		iTick;
	Matrix	mat;
};
class SkinData
{
public:
	static int						iNumMaxWeight;
	vector<FbxNode*>				InfluenceNodes;
	unordered_map<T_STR, Matrix>	m_matBindPoseMap;

	size_t							dwVertexCount;
	DWORD							dwVertexStride;
	unique_ptr<int[]>				pBoneIndices;
	unique_ptr<float[]>				pBoneWeights;
public:
	void	Alloc(size_t dwCount, DWORD dwStride);
	int*	GetIndices(size_t dwIndex);
	float*	GetWeights(size_t dwIndex);
	DWORD	GetBoneCount() const;
	void	InsertWeight(size_t dwIndex, DWORD dwBoneIndex, float fBoneWeight);
public:
	SkinData() : dwVertexCount(0), dwVertexStride(0) {}
	~SkinData() {}
};

class SModelObject : public TObject
{
public:
	vector<wstring>		fbxMaterialList;
	vector<subMesh>		m_subMesh;
	vector<SAnimTrack>	animlist;
	vector<SWeight>		WeightList;
	bool				m_bSkinnedMesh = false;
	virtual ~SModelObject()
	{

	}
};
typedef std::unordered_map<FbxNode*, SModelObject*>	sMeshMap;
typedef std::vector<SModelObject*>	sMeshList;
class SFbxObj
{
public:
	SScene				m_Scene;
	float				m_fTick = 0;
	static FbxManager*	g_pSDKManager;
	FbxImporter*		m_pFbxImporter;
	FbxScene*			m_pFBXScene;
	unordered_map<string, Matrix> m_dxMatrixMap;
	unordered_map<FbxNode*, int> m_pNodeMap;
	vector<Matrix> m_pMatrixList;
	sMeshMap			m_sMeshMap;
	sMeshList			m_sMeshList;
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
	FbxColor ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* pVertexColorSet, DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4 ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	void ParseAnimStack(FbxScene * FBXScene, FbxString* AnimStackNameArray);
	void ParseNodeAnimation(FbxNode* Node);
	////////
	bool ParseMeshSkinning(const FbxMesh* pFbxMesh, SkinData* skindata);
	bool ParseMeshSkinningMap(const FbxMesh* pFbxMesh, vector< SWeight>& skindata);
public:
	SFbxObj();
};

