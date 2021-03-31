#include "SFbxObj.h"

void SFbxObj::ParseAnimat(FbxScene * FBXScene)
{
	FbxArray<FbxString*>  AnimStackNameArray;
	FBXScene->FillAnimStackNameArray(AnimStackNameArray);
	int iAnimStackCount = AnimStackNameArray.GetCount();
	for (int i = 0; i < iAnimStackCount; i++)
	{
		ParseAnimStack(FBXScene, AnimStackNameArray.GetAt(i));
	}
}
void SFbxObj::ParseAnimation(FbxScene*	pFbxScene)
{
	FbxArray<FbxString*>  AnimStackNameArray;
	pFbxScene->FillAnimStackNameArray(AnimStackNameArray);
	int iAnimStackCount = AnimStackNameArray.GetCount();
	for (int i = 0; i < iAnimStackCount; i++)
	{
		ParseAnimStack(pFbxScene, AnimStackNameArray.GetAt(i));
	}
}
void SFbxObj::ParseAnimStack(FbxScene * FBXScene, FbxString* AnimStackNameArray)
{
	FbxAnimStack* anim = FBXScene->FindMember<FbxAnimStack>(AnimStackNameArray->Buffer());
	if (anim == nullptr) return;
	FbxTakeInfo* info = FBXScene->GetTakeInfo(*AnimStackNameArray);

	FbxTime FrameTime;
	FrameTime.SetTime(0, 0, 0, 1, 0, FBXScene->GetGlobalSettings().GetTimeMode());
	float fFrameTime = FrameTime.GetSecondDouble();
	float fStartTime, fEndTime;
	if (info)
	{
		// scene
		fStartTime = info->mLocalTimeSpan.GetStart().GetSecondDouble();
		fEndTime = info->mLocalTimeSpan.GetStop().GetSecondDouble();
	}
	else
	{
		FbxTimeSpan tlTimeSpan;
		FBXScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(tlTimeSpan);

		fStartTime = (float)tlTimeSpan.GetStart().GetSecondDouble();
		fEndTime = (float)tlTimeSpan.GetStop().GetSecondDouble();
	}

	m_Scene.iFirstFrame = fStartTime * 30.0f;
	m_Scene.iLastFrame = fEndTime * 30.0f;
	m_Scene.iFrameSpeed = 30;
	m_Scene.iTickPerFrame = 160;
	m_Scene.iDeltaTick = 1;
	m_Scene.fDeltaTime = fFrameTime * 1.0f;
	m_Scene.fFirstTime = fStartTime;
	m_Scene.fLastTime = fEndTime;
}

void SFbxObj::ParseNodeAnimation(FbxNode* Node)
{
	if (Node == nullptr) return;
	if (Node && (Node->GetCamera() || Node->GetLight()))
	{
		return;
	}
#if (FBXSDK_VERSION_MAJOR > 2014 || ((FBXSDK_VERSION_MAJOR==2014) && (FBXSDK_VERSION_MINOR>1) ) )
	auto anim = m_pFBXScene->GetAnimationEvaluator();
#else
	auto anim = m_pFBXScene->GetEvaluator();
#endif
	float fCurrentTime = 0.0f;
	int a = 0;
	while (fCurrentTime <= m_Scene.fLastTime)
	{
		//FbxTime t;
		//t.SetSecondDouble(fCurrentTime);
		//FbxAMatrix mat = anim->GetNodeGlobalTransform(Node, t);
		//SAnimTrack track;
		//track.iTick = fCurrentTime * 30 * 160;
		//track.mat = DxConvertMatrix(ConvertMatrixA(mat));
		//fCurrentTime += m_Scene.fDeltaTime;
		//auto data = m_sNodeMap.find(Node);
		//if (data->second->animlist.empty())
		//{
		//	data->second->animlist.push_back(track);
		//	a++;
		//	continue;
		//}
		//if (track.mat != data->second->animlist[a - 1].mat)
		//{
		//	data->second->animlist.push_back(track);
		//	a++;
		//}
		FbxTime t;
		t.SetSecondDouble(fCurrentTime);
		FbxAMatrix mat = anim->GetNodeGlobalTransform(Node, t);
		SAnimTrack track;
		track.iTick = fCurrentTime * 30 * 160;
		track.mat = DxConvertMatrix(ConvertMatrixA(mat));

		auto data = m_sNodeMap.find(Node);
		data->second->animlist.push_back(track);
		fCurrentTime += m_Scene.fDeltaTime;
		// mat 1차 부모행렬 역행렬 곱한다.
		FbxAMatrix self;
		// self 2차 행렬 분해( S, R, T )
		// 3차 에니메이션 트랙 행렬
		// S(벡터), R(쿼터니언), T(벡터)를 샘플링 시간 간격으로 저장
		
	}
	int dwChild = Node->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = Node->GetChild(dwObj);
		ParseNodeAnimation(pChildNode);
	}
}

//bool SFbxObj::ParseMeshSkinning(const FbxMesh* pFbxMesh, SkinData* skindata)
//{
//	int iDeformerCount = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
//	if (iDeformerCount == 0)
//	{
//		return false;
//	}
//	int iVertexCount = pFbxMesh->GetControlPointsCount();
//	skindata->Alloc(iVertexCount, 8);
//
//	for (int dwDeformerIndex = 0; dwDeformerIndex < iDeformerCount; dwDeformerIndex++)
//	{
//		auto pSkin = reinterpret_cast<FbxSkin*>(pFbxMesh->GetDeformer(dwDeformerIndex, FbxDeformer::eSkin));
//		DWORD dwClusterCount = pSkin->GetClusterCount();
//		for (int dwClusterIndex = 0; dwClusterIndex < dwClusterCount; dwClusterIndex++)
//		{
//			auto pCluster = pSkin->GetCluster(dwClusterIndex);
//			int dwClusterSize = pCluster->GetControlPointIndicesCount();
//			auto pLink = pCluster->GetLink();
//
//			int iBoneIndex = skindata->GetBoneCount();
//			skindata->InfluenceNodes.push_back(pLink);
//			int* pIndices = pCluster->GetControlPointIndices();
//			double* pWeights = pCluster->GetControlPointWeights();
//			for (int i = 0; i < dwClusterSize; i++)
//			{
//				skindata->InsertWeight(pIndices[i],iBoneIndex, pWeights[i]);
//			}
//		}
//	}
//	return true;
//}

bool SFbxObj::ParseMeshSkinningMap(const FbxMesh* pFbxMesh, vector< SWeight>& skindata)
{
	int iDeformerCount = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iDeformerCount == 0)
	{
		return false;
	}
	int iVertexCount = pFbxMesh->GetControlPointsCount();
	skindata.resize(iVertexCount);
	for (int dwDeformerIndex = 0; dwDeformerIndex < iDeformerCount; dwDeformerIndex++)
	{
		auto pSkin = reinterpret_cast<FbxSkin*>(pFbxMesh->GetDeformer(dwDeformerIndex, FbxDeformer::eSkin));
		DWORD dwClusterCount = pSkin->GetClusterCount();
		for (int dwClusterIndex = 0; dwClusterIndex < dwClusterCount; dwClusterIndex++)
		{
			auto pCluster = pSkin->GetCluster(dwClusterIndex);
			////
			FbxAMatrix matXBindPose;
			FbxAMatrix matReferenceGlobalInitPosition;
			pCluster->GetTransformLinkMatrix(matXBindPose);
			pCluster->GetTransformMatrix(matReferenceGlobalInitPosition);
			FbxMatrix matBindPose = matReferenceGlobalInitPosition.Inverse() * matXBindPose;

			Matrix matInvBindPos = DxConvertMatrix(ConvertMatrixA(matBindPose));
			matInvBindPos = matInvBindPos.Invert();

			m_dxMatrixMap.insert(make_pair(pCluster->GetLink()->GetName(), matInvBindPos));

			int  dwClusterSize = pCluster->GetControlPointIndicesCount();
			auto data = m_pNodeMap.find(pCluster->GetLink());
			int  iBoneIndex = data->second;
			// 영향을 받는 정점들의 인덱스
			int* pIndices = pCluster->GetControlPointIndices();
			double* pWeights = pCluster->GetControlPointWeights();
			for (int i = 0; i < dwClusterSize; i++)
			{
				skindata[pIndices[i]].InsertWeight(iBoneIndex, pWeights[i]);
			}
		}
	}
	return true;
}