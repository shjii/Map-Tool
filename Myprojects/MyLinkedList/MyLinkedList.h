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

	// ����
	void Ascending();
	// ����
	void Descending();
	//����
	Node<T>* UpdateData(Node<T>* upDataData);
	//�˻�
	Node<T>* SearchNode(char* Data);
	//���
	void PrintNode(Node<T>* printN);
	//��ü ���
	void AllPrintNode();
	//����
	Node<T>* DelNode(Node<T>* lDeT);
	// ��ü ����
	void AllDelNode();
	//��ũ ����
	void AddLink(Node<T>* NewLink);
	void AddLink(T* NewLink);
	//����
	void Link(Node<T>* NewLink);
	//����
	void Produce(int step);
	//�ʱ�ȭ
	void reset();
	void LoadLink(const char* FileName);
	// ���� ����
	void SaveData(const char* FileName);
};

// ����
template <class T>
void MyClass<T>::Ascending()
{
	Node<T>* start; // ���� ��ġ
	Node<T>* pcompare; // �˻� 1��
	Node<T>* ppcompare; // �˻� 2��
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
// ����
template <class T>
void MyClass<T>::Descending()
{
	Node<T>* start; // ���� ��ġ
	Node<T>* pcompare; // �˻� 1��
	Node<T>* ppcompare; // �˻� 2��
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
//����
template <class T>
Node<T>* MyClass<T>::UpdateData(Node<T>* upDataData)
{
	if (upDataData == NULL)  return NULL;

	char a[20] = { 0 };
	int b[3];

	printf("�̸� : ");
	scanf("%s", &a);
	printf("���� : ");
	scanf("%d", &b[0]);
	printf("���� : ");
	scanf("%d", &b[1]);
	printf("���� : ");
	scanf("%d", &b[2]);;

	strcpy(upDataData->IData->lName, a);
	upDataData->IData->lKor = b[0];
	upDataData->IData->lEng = b[1];
	upDataData->IData->lMat = b[2];
	upDataData->IData->lTotal = b[0] + b[1] + b[2];
	upDataData->IData->lAverage = upDataData->IData->lTotal / 3.0f;
	return upDataData;
}
//�˻�
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
//���
template <class T>
void MyClass<T>::PrintNode(Node<T>* printN)
{
	//printf("�̸� : %s ���� : %d ���� : %d ���� : %d �� : %d ��� :%0.5f \n", printN->IData->lName, printN->IData->lKor, printN->IData->lEng, printN->IData->lMat, printN->IData->lTotal, printN->IData->lAverage);
	printN->PrintNode();
	return;
}
//��ü ���
template <class T>
void MyClass<T>::AllPrintNode()
{
	for (Node<T>* printNodetmp = myListHede->lNext; printNodetmp != myListTail; printNodetmp = printNodetmp->lNext)
	{
		PrintNode(printNodetmp);
	}
	return;
}
//����
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
// ��ü ����
template <class T>
void MyClass<T>::AllDelNode()
{
	Node<T>* lDeT = myListHede->lNext;

	while (lDeT != myListTail)
	{
		lDeT = DelNode(lDeT);
	}

}
//��ũ ����
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
//����
template <class T>
void MyClass<T>::Link(Node<T>* NewLink)
{
	//�˻��� �ڷ�
	Node<T>* LTmp = LFindNode->lNext;
	//�˻��� ������
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
//����
template <class T>
void MyClass<T>::Produce(int step)
{

	Node<T>* NewLT = new Node<T>;
	T* NewL= new T;
		printf("�̸� : ");
		scanf("%s", &NewL->lName);
		printf("���� : ");
		scanf("%d", &NewL->lKor);
		printf("���� : ");
		scanf("%d", &NewL->lEng);
		printf("���� : ");
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
//�ʱ�ȭ
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
// ���� ����
template <class T>
void MyClass<T>::SaveData(const char* FileName)
{
	//MyClass myClass;
	int a = 0;
	FILE* fp_dest = fopen(FileName, "w");
	Node<T>* nodeWrite = 0;
	if (nodeCunt <= 0)
	{
		printf("\n �����Ͱ� �����ϴ�.");
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