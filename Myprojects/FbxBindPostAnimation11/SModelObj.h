#pragma once
#include <TStd.h>
#include <fbxsdk.h>
#include <unordered_map>
#include <TObject.h>

struct SSubMesh
{
	std::vector<STri>			m_TriangleList;
	std::vector<PNCT_VERTEX>	m_VertexList;
	std::vector<IW_VERTEX>		m_VertexListIW;
	ComPtr<ID3D11Buffer>		m_pVertexBuffer;
	ComPtr<ID3D11Buffer>		m_pVertexBufferIW;
	ComPtr<ID3D11Buffer>		m_pIndexBuffer;
	STexture*					m_pTexture;
	int							m_iNumFace = 0;
	
	vector<DWORD>				m_IndexArray;

	void						SetUniqueBuffer(STri& trl);

	~SSubMesh()
	{
		m_iNumFace = 0;
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
				for (DWORD j = (Index.size() - 1); j > i; --j)
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
	int			iTick;
	Vector3		p;
	Vector3		s;
	Quaternion	q;
	Matrix		mat;
};

class SModelObj : public TObject
{
public:
	SModelObj*  m_pParentObject = nullptr;
	Matrix		m_matAnim;
public:
	vector<wstring>		fbxMaterialList;
	vector<SSubMesh>	subMesh;
	vector<SAnimTrack>	animlist;
	vector<SWeight>		WeightList;
	unordered_map<string, Matrix>	m_dxMatrixBindPoseMap;
	bool				m_bSkinnedMesh = false;
public:
	bool				CreateInputLayout() override;
public:
	SModelObj()
	{
	}
	virtual ~SModelObj()
	{

	}
};