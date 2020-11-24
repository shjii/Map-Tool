#include "Sample.h"

enum iWorS
{

};

TNode* UpdateData(TNode* pFindNode)
{
	if (g_pFindNode == NULL)  return NULL;

	srand(time(NULL));
	int subject[3];
	for (int i = 0; i < 3; i++)
	{
		subject[i] = rand() % 101;
	}
	int iNameA = 65 + rand() % 26;
	int iNameB = 65 + rand() % 26;
	int iNameC = 65 + rand() % 26;
	char name[4] = { 0, };
	name[0] = iNameA;
	name[1] = iNameB;
	name[2] = iNameC;

	strcpy(pFindNode->m_szName, name);
	pFindNode->m_iKor = subject[0];
	pFindNode->m_iEng = subject[1];
	pFindNode->m_iMat = subject[2];

	return pFindNode;
}

TNode* FindName(char* pFindName)
{
	TNode* nodeWrite = 0;
	TNode* nodePrev = 0;
	for (nodeWrite = g_pNodeHead; nodeWrite != NULL; nodeWrite = nodeWrite->pNext)
	{
		if (nodeWrite->pNext != NULL && !strcmp(nodeWrite->pNext->m_szName, pFindName))
		{
			return nodeWrite;
		}
	}
	return NULL;
}

void PrintData(TNode* pFindNode)
{
	if (pFindNode == NULL) return;
	pFindNode->m_iTotal = pFindNode->m_iKor +
		pFindNode->m_iEng +
		pFindNode->m_iMat;
	pFindNode->m_fAverage = pFindNode->m_iTotal / 3.0f;
	printf("\n%s %d %d %d %d %d %10.4f", pFindNode->m_szName,
		pFindNode->m_iAge,
		pFindNode->m_iKor,
		pFindNode->m_iEng,
		pFindNode->m_iMat,
		pFindNode->m_iTotal,
		pFindNode->m_fAverage);
}

bool InsertLink(TNode* pFindNode, char* pName, int m_iAge, int* pData)
{
	if (pFindNode == NULL) return false;
	TNode* pNewData = new TNode;
	strcpy(pNewData->m_szName, pName);
	pNewData->m_iKor = pData[0];
	pNewData->m_iEng = pData[1];
	pNewData->m_iMat = pData[2];
	pNewData->pNext = 0;
	//중간 삽입
	TNode* pTempNode = pFindNode->pNext;
	pFindNode->pNext = pNewData;
	pNewData->pNext = pTempNode;
	return true;
}

void LoadLink(const char* pLoadFileName)
{
	AllDeleteLink();

	FILE* fp_dest = fopen(pLoadFileName, "r");
	if (fp_dest != NULL)
	{
		int iCounter;
		fscanf(fp_dest, "%d", &iCounter);
		for (int i = 0; i < iCounter; i++)
		{
			TNode nodeRead;
			nodeRead.pNext = 0;
			fscanf(fp_dest, "%s %d %d %d %d",
				nodeRead.m_szName,
				&nodeRead.m_iAge,
				&nodeRead.m_iKor,
				&nodeRead.m_iEng,
				&nodeRead.m_iMat);
			
			int Data[3];
			Data[0] = nodeRead.m_iKor;
			Data[1] = nodeRead.m_iEng;
			Data[2] = nodeRead.m_iMat;
			NewLink(nodeRead.m_szName, nodeRead.m_iAge, Data);
		}
	}
	fclose(fp_dest);
}

void SaveData(const char* pSaveFileName)
{
	int a = 0;
	FILE* fp_dest = fopen(pSaveFileName, "w");
	TNode* nodeWrite = 0;
	if (g_iNumCounter <= 0)
	{
		printf("\n 데이터가 없습니다.");
	}
	fprintf(fp_dest, "%d", g_iNumCounter);

	for (nodeWrite = g_pNodeHead->pNext; nodeWrite != NULL; nodeWrite = nodeWrite->pNext)
	{
		fprintf(fp_dest, "\n%s %d %d %d %d", nodeWrite->m_szName,
			nodeWrite->m_iAge,
			nodeWrite->m_iKor, nodeWrite->m_iEng, nodeWrite->m_iMat);
	}
	fclose(fp_dest);
}

//삭제
TNode* DelLink(TNode* pPrevNode)
{
	TNode* pDleNode = pPrevNode->pNext;
	TNode* pNextNode = pDleNode->pNext;
	delete pDleNode;
	g_iNumCounter--;

	pPrevNode->pNext = pNextNode;
	if (g_pNodeHead->pNext == g_pNodeTail)
	{
		g_pNodeTail = g_pNodeHead;
	}
	return pNextNode;
}

//리스트 해제
void AllDeleteLink()
{
	while (g_pNodeHead->pNext != NULL)
	{
		DelLink(g_pNodeHead);
	}
	g_pNodeTail = g_pNodeHead;
}

//모든 데이터 출력
void PrintAllData()
{
	TNode* nodeWrite = g_pNodeHead->pNext;
	if (nodeWrite == NULL)
	{
		printf("\n 데이터 없음!");
		return;
	}
	while (nodeWrite != NULL)
	{
		nodeWrite->m_iTotal = nodeWrite->m_iEng + nodeWrite->m_iKor + nodeWrite->m_iMat;
		nodeWrite->m_fAverage = nodeWrite->m_iTotal / 3.0f;
		printf("\n 이름 : %s 국어 : %d 영어 : %d 수학 : %d 합 : %d 평균 : %10.4f",
			nodeWrite->m_szName, nodeWrite->m_iKor, nodeWrite->m_iEng, nodeWrite->m_iMat,nodeWrite->m_iTotal,nodeWrite->m_fAverage);
		nodeWrite = nodeWrite->pNext;
	}
}

// 링크 구현 (헤드 -> newDtat(꼬리),  헤드 -> newData0 -> newData1(꼬리)
void NewLink(char* pName, int pAge, int* pData)
{
	TNode* pNewData = new TNode;
	strcpy(pNewData->m_szName, pName);
	pNewData->m_iAge = pAge;
	pNewData->m_iKor = pData[0];
	pNewData->m_iEng = pData[1];
	pNewData->m_iMat = pData[2];
	pNewData->pNext = 0;
	g_pNodeTail->pNext = pNewData;
	g_pNodeTail = pNewData;
	g_iNumCounter++;
}

//  랜덤함수 생성
void RandData(int piWor)
{
	srand(time(NULL));
	int subject[3];
	for (int i = 0; i < 3; i++)
	{
		subject[i] = rand() % 101;
	}
	int iNameA = 65 + rand() % 26;
	int iNameB = 65 + rand() % 26;
	int iNameC = 65 + rand() % 26;
	char name[4] = { 0, };
	name[0] = iNameA;
	name[1] = iNameB;
	name[2] = iNameC;
	
	if (g_pFindNode != NULL && piWor == 3)
	{
		InsertLink(g_pFindNode->pNext, name, rand() % 100, subject);
	}
	else
	{
		NewLink(name, rand() % 100, subject);
	}
}

//초기화
bool Initialize()
{
	g_pNodeHead = new TNode;
	memset(g_pNodeHead, 0, sizeof(TNode));
	g_pNodeTail = g_pNodeHead;
	srand(time(NULL));
	if (g_pNodeHead != NULL) return true;
	return false;
}


int main()
{
	if (!Initialize())
	{
		printf("초기화에 실패했습니다\n");
		return 1;
	}
	bool bRunning = true;
	while (bRunning)
	{
		int iWor = 0;
		printf("\n생성(0),출력(1),저장(2),추가(3),로드(4),삭제(5),검색(6),수정(7),종료(99) :");
		scanf("%d", &iWor);
		switch (iWor)
		{
		case 0:
		{
			system("cls");
			printf("\n 새로운 Data 추가");
			RandData(iWor);
		}break;
		case 1:
		{
			system("cls");
			printf("\n 출력");
			PrintAllData();
		}break;
		case 2:
		{
			SaveData("save.txt");
		}break;
		case 3:
		{
			RandData(iWor);
		}break;
		case 4:
		{
			LoadLink("save.txt");
		}break;
		case 5:
		{
			if (g_pFindNode == NULL)
			{
				char strFileName[20] = { 0 };
				printf("\n 이름ㅇ르 입력하시오");
				scanf("%s", strFileName);
				g_pFindNode = FindName(strFileName);
			}
			DelLink(g_pFindNode);
		}break;

		case 6:
		{
			char strFileName[20] = { 0 };
			printf("\n 이름을 입력하시오 : ");
			scanf("%s", strFileName);
			g_pFindNode = FindName(strFileName);
			if (g_pFindNode != NULL)
			{
				PrintData(g_pFindNode->pNext);
			}
			else
			{
				printf("검색 오류!");
			}
		}break;

		case 7:
		{	
			if (g_pFindNode == NULL)
			{
				char strFileName[20] = { 0 };
				printf("\n 이름ㅇ르 입력하시오");
				scanf("%s", strFileName);
				g_pFindNode = FindName(strFileName);
			}
			UpdateData(g_pFindNode->pNext);
		}break;

		case 99:
		{
			bRunning = false;
			AllDeleteLink();
		}break;

		default:
		{
			printf("\n 다시 입력");
		}break;
		}
	}
	delete g_pNodeHead;
	g_pNodeHead = 0;
}