#pragma once
#include <TStd.h>
#include <fbxsdk.h>
#include <unordered_map>
#include <TObject.h>

struct subMesh
{
	std::vector<STri>			m_TriangleList;
	std::vector<PNCT_VERTEX>	m_VertexList;
	std::vector<IW_VERTEX>		m_VertexListIW;
	ComPtr<ID3D11Buffer>		m_pVertexBuffer;
	ComPtr<ID3D11Buffer>		m_pVertexBufferIW;
	STexture*					m_pTexture;
	int							m_count;
	~subMesh()
	{
		m_count = 0;
	}
};
struct SWeight
{
	vector<int> Index;
	vector<float> Weight;
	void InsertWeight(int iBoneIndex, float fBoneWeight)
	{
		for (DWORD i = 0; i < Index.size(); i++)
		{
			if (fBoneWeight > Weight[i])
			{
				for (DWORD j = 0; j < (Index.size() - 1); j++)
				{
					Index[j] = Index[j - 1];
					Weight[j] = Weight[j - 1];
				}
				Index[i] = iBoneIndex;
				Weight[i] = fBoneWeight;
				break;
			}
		}
	}
	SWeight()
	{
		Index.resize(8);
		Weight.resize(8);
	}
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

class SModelObj : public TObject
{
public:
	vector<wstring>		fbxMaterialList;
	vector<subMesh>		m_subMesh;
	vector<SAnimTrack>	animlist;
	vector<SWeight>		WeightList;
	bool				m_bSkinnedMesh = false;
public:
	bool				CreateInputLayout() override;
public:
	virtual ~SModelObj()
	{

	}
};