#pragma once
#include "Node.h"

template <class T>
class MyClass
{
public:
	int nodeCunt = 0;
	Node<T>* LFindNode;
	Node<T>* myListHede = 0;
	Node<T>* myListTail = 0;

	// 정렬
	void Ascending();
	// 정렬
	void Descending();
	//수정
	Node<T>* UpdateData(Node<T>* upDataData);
	//검색
	Node<T>* SearchNode(char* Data);
	//출력
	void PrintNode(Node<T>* printN);
	//전체 출력
	void AllPrintNode();
	//삭제
	Node<T>* DelNode(Node<T>* lDeT);
	// 전체 삭제
	void AllDelNode();
	//링크 구현
	void AddLink(Node<T>* NewLink);
	void AddLink(T* NewLink);
	//삽입
	void Link(Node<T>* NewLink);
	//생성
	void Produce(int step);
	//초기화
	void reset();
	void LoadLink(const char* FileName);
	// 파일 저장
	void SaveData(const char* FileName);
};

// 정렬
template <class T>
void MyClass<T>::Ascending()
{
	Node<T>* start; // 시작 위치
	Node<T>* pcompare; // 검사 1번
	Node<T>* ppcompare; // 검사 2번
	Node<T>* tmpPrev;
	Node<T>* tmpNext;
	start = myListHede->lNext;
	pcompare = start;
	ppcompare = pcompare;
	for (int i = 0; i < nodeCunt; i++)
	{
		for (ppcompare; ppcompare != myListTail; ppcompare = ppcompare->lNext)
		{
			if (pcompare->IData->lTotal < ppcompare->IData->lTotal)
			{
				pcompare = ppcompare;
			}
		}
		if (pcompare == start)
		{
			start = start->lNext;
		}
		ppcompare = start;
		tmpPrev = pcompare->lPrev;
		tmpNext = pcompare->lNext;
		if (tmpPrev != NULL)
		{
			tmpNext->lPrev = tmpPrev;
		}
		if (tmpNext != NULL)
		{
			tmpPrev->lNext = tmpNext;
		}
		AddLink(pcompare);
		pcompare = start;
	}
}
// 정렬
template <class T>
void MyClass<T>::Descending()
{
	Node<T>* start; // 시작 위치
	Node<T>* pcompare; // 검사 1번
	Node<T>* ppcompare; // 검사 2번
	Node<T>* tmpPrev;
	Node<T>* tmpNext;
	start = myListHede->lNext;
	pcompare = start;
	ppcompare = pcompare;
	for (int i = 0; i < nodeCunt; i++)
	{
		for (ppcompare; ppcompare != myListTail; ppcompare = ppcompare->lNext)
		{
			if (pcompare->IData->lTotal > ppcompare->IData->lTotal)
			{
				pcompare = ppcompare;
			}
		}
		if (pcompare == start)
		{
			start = start->lNext;
		}
		ppcompare = start;
		tmpPrev = pcompare->lPrev;
		tmpNext = pcompare->lNext;
		if (tmpPrev != NULL)
		{
			tmpNext->lPrev = tmpPrev;
		}
		if (tmpNext != NULL)
		{
			tmpPrev->lNext = tmpNext;
		}
		AddLink(pcompare);
		pcompare = start;
	}
}
//수정
template <class T>
Node<T>* MyClass<T>::UpdateData(Node<T>* upDataData)
{
	if (upDataData == NULL)  return NULL;

	char a[20] = { 0 };
	int b[3];

	printf("이름 : ");
	scanf("%s", &a);
	printf("국어 : ");
	scanf("%d", &b[0]);
	printf("영어 : ");
	scanf("%d", &b[1]);
	printf("수학 : ");
	scanf("%d", &b[2]);;

	strcpy(upDataData->IData->lName, a);
	upDataData->IData->lKor = b[0];
	upDataData->IData->lEng = b[1];
	upDataData->IData->lMat = b[2];
	upDataData->IData->lTotal = b[0] + b[1] + b[2];
	upDataData->IData->lAverage = upDataData->IData->lTotal / 3.0f;
	return upDataData;
}
//검색
template <class T>
Node<T>* MyClass<T>::SearchNode(char* Data)
{
	Node<T>* searchDatatmp = myListHede->lNext;
	for (searchDatatmp; searchDatatmp != myListTail; searchDatatmp = searchDatatmp->lNext)
	{
		if (searchDatatmp != NULL && !strcmp(searchDatatmp->IData->lName, Data))
		{
			return searchDatatmp;
		}
	}
	return NULL;
}
//출력
template <class T>
void MyClass<T>::PrintNode(Node<T>* printN)
{
	//printf("이름 : %s 국어 : %d 영어 : %d 수학 : %d 합 : %d 평균 :%0.5f \n", printN->IData->lName, printN->IData->lKor, printN->IData->lEng, printN->IData->lMat, printN->IData->lTotal, printN->IData->lAverage);
	printN->PrintNode();
	return;
}
//전체 출력
template <class T>
void MyClass<T>::AllPrintNode()
{
	for (Node<T>* printNodetmp = myListHede->lNext; printNodetmp != myListTail; printNodetmp = printNodetmp->lNext)
	{
		PrintNode(printNodetmp);
	}
	return;
}
//삭제
template <class T>
Node<T>* MyClass<T>::DelNode(Node<T>* lDeT)
{
	Node<T>* pDel = lDeT->lNext;
	Node<T>* ppDel = lDeT->lPrev;

	pDel->lPrev = ppDel;
	ppDel->lNext = pDel;
	delete lDeT;
	lDeT = NULL;
	nodeCunt--;
	return pDel;
}
// 전체 삭제
template <class T>
void MyClass<T>::AllDelNode()
{
	Node<T>* lDeT = myListHede->lNext;

	while (lDeT != myListTail)
	{
		lDeT = DelNode(lDeT);
	}

}
//링크 구현
template <class T>
void MyClass<T>::AddLink(Node<T>* NewLink)
{
	Node<T>* LTmp = myListHede->lNext;
	Node<T>* NewL = NewLink;
	myListHede->lNext = NewL;
	if (LTmp != NULL)
	{
		LTmp->lPrev = NewL;
	}
	else
	{
		myListTail->lPrev = NewL;
	}
	NewL->lNext = LTmp;
	NewL->lPrev = myListHede;
}
template <class T>
void MyClass<T>::AddLink(T* NewLink)
{
	Node<T>* LTmp = myListHede->lNext;
	Node<T>* NewL = new Node<T>;
	NewL->IData = NewLink;
	myListHede->lNext = NewL;
	if (LTmp != NULL)
	{
		LTmp->lPrev = NewL;
	}
	else
	{
		myListTail->lPrev = NewL;
	}
	NewL->lNext = LTmp;
	NewL->lPrev = myListHede;
}
//삽입
template <class T>
void MyClass<T>::Link(Node<T>* NewLink)
{
	//검색값 뒤로
	Node<T>* LTmp = LFindNode->lNext;
	//검색갑 앞으로
	//T* LTmp = LFindNode->lNext;
	LFindNode->lNext = NewLink;
	NewLink->lNext = LTmp;
	NewLink->lPrev = LFindNode;
	if (LTmp != NULL)
	{
		LTmp->lPrev = NewLink;
	}
	else
	{
		myListTail->lPrev = NewLink;
		//myListHede->INext = NewLink;
	}
}
//생성
template <class T>
void MyClass<T>::Produce(int step)
{

	Node<T>* NewLT = new Node<T>;
	T* NewL= new T;
		printf("이름 : ");
		scanf("%s", &NewL->lName);
		printf("국어 : ");
		scanf("%d", &NewL->lKor);
		printf("영어 : ");
		scanf("%d", &NewL->lEng);
		printf("수학 : ");
		scanf("%d", &NewL->lMat);
		NewL->lTotal = NewL->lKor + NewL->lEng + NewL->lMat;
		NewL->lAverage = NewL->lTotal / 3.0f;

		NewLT->IData = NewL;
	if (step != 5)
	{
		AddLink(NewLT);
	}
	else
	{
		Link(NewLT);
	}
	nodeCunt++;
}
//초기화
template <class T>
void MyClass<T>::reset()
{
	myListHede = new Node<T>;
	myListTail = new Node<T>;
	memset(myListHede, 0, sizeof(T));
	memset(myListTail, 0, sizeof(T));
	myListHede->lNext = myListTail;
	myListTail->lPrev = myListHede;
}
template <class T>
void MyClass<T>::LoadLink(const char* FileName)
{
	//MyClass myClass;

	AllDelNode();

	FILE* fp_dest = fopen(FileName, "r");
	if (fp_dest != NULL)
	{
		int iCounter;
		fscanf(fp_dest, "%d", &iCounter);
		for (int i = 0; i < iCounter; i++)
		{
			T* nodeRead = new T;
			fscanf(fp_dest, "%s %d %d %d",
				&nodeRead->lName,
				&nodeRead->lKor,
				&nodeRead->lEng,
				&nodeRead->lMat);
			nodeRead->lTotal = nodeRead->lKor +
				nodeRead->lEng +
				nodeRead->lMat;
			nodeRead->lAverage = nodeRead->lTotal / 3.0f;

			AddLink(nodeRead);
			nodeCunt++;
		}
	}
	fclose(fp_dest);
}
// 파일 저장
template <class T>
void MyClass<T>::SaveData(const char* FileName)
{
	//MyClass myClass;
	int a = 0;
	FILE* fp_dest = fopen(FileName, "w");
	Node<T>* nodeWrite = 0;
	if (nodeCunt <= 0)
	{
		printf("\n 데이터가 없습니다.");
	}
	fprintf(fp_dest, "%d", nodeCunt);

	for (nodeWrite = myListHede->lNext; nodeWrite != myListTail; nodeWrite = nodeWrite->lNext)
	{
		fprintf(fp_dest, "\n%s %d %d %d", nodeWrite->IData->lName,
			nodeWrite->IData->lKor,
			nodeWrite->IData->lEng, nodeWrite->IData->lMat);
	}
	fclose(fp_dest);
}