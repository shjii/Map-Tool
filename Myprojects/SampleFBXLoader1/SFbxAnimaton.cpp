#include "SFbxObj.h"

void SFbxObj::ParseAnimat(FbxScene * FBXScene)
{
	FbxArray<FbxString*> AnimStackNameArray;
	m_pFBXScene->FillAnimStackNameArray(AnimStackNameArray);
	int iAnimStackCount = AnimStackNameArray.GetCount();
	for (int i = 0; i < iAnimStackCount; i++)
	{
		ParseAnimStack(m_pFBXScene, AnimStackNameArray.GetAt(i));
	}
}
void SFbxObj::ParseAnimStack(FbxScene * FBXScene, FbxString* AnimStackNameArray)
{
	FbxAnimStack* anim = FBXScene->FindMember<FbxAnimStack>(AnimStackNameArray->Buffer());
	if (anim == nullptr) return;
	FbxTakeInfo* Info = FBXScene->GetTakeInfo(*AnimStackNameArray);

	FbxTime FrameTime;
	FrameTime.SetTime(0,0,0,1,0,FBXScene->GetGlobalSettings().GetTimeMode());
	float fFrameTime = FrameTime.GetSecondDouble();
	float fStartTime, fEndTime;
	if (Info)
	{
		fStartTime = Info->mLocalTimeSpan.GetStart().GetSecondDouble();
		fEndTime = Info->mLocalTimeSpan.GetStop().GetSecondDouble();
	}
	else
	{
		FbxTimeSpan tITimeSpan;
		m_pFBXScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(tITimeSpan);

		fStartTime = (float)tITimeSpan.GetStart().GetSecondDouble();
		fEndTime = (float)tITimeSpan.GetStop().GetSecondDouble();
	}

	m_Scene.iFirstFrame = fStartTime * 30.0f;
	m_Scene.iLastFrame = fEndTime * 30.0f;
	m_Scene.iFrameSpeed = 30;
	m_Scene.iTickPerFrame = 160;
	m_Scene.iDeltaTick = 1;
	m_Scene.fDeltaTime = fFrameTime * 1.0f;
	m_Scene.fFirstTime = fStartTime;
	m_Scene.fLastTime = fEndTime;
	ParseNodeAnimation(FBXScene->GetRootNode());
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
	while (fCurrentTime <= m_Scene.fLastTime)
	{
		FbxTime t;
		t.SetSecondDouble(fCurrentTime);
		FbxAMatrix mat = anim->GetNodeGlobalTransform(Node, t);
		SAnimTrack track;
		track.iTick = fCurrentTime * 30 * 160;
		track.mat = DxConvertMatrix(ConvertMatrixA(mat));

		auto data = m_sMeshMap.find(Node);
		data->second->animlist.push_back(track);
		fCurrentTime += m_Scene.fDeltaTime;
	}
	int dwChild = Node->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = Node->GetChild(dwObj);
		ParseNodeAnimation(pChildNode);
	}
}