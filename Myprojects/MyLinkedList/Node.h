#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

template <class T>
class Node
{
public:

	T* IData;
	char IName[20];
	Node* lPrev;
	Node* lNext;
	virtual void PrintNode(){}
	bool operator>(Node& NextData);
	bool operator<(Node& NextData);
	bool operator==(Node& NextData);
	~Node() {
		delete(this->IData);
		this->IData = nullptr;
	}
};

class LNode{
public:
	char lName[20];
	int lKor;
	int lEng;
	int lMat;
	int lTotal;
	float lAverage;
	void PrintNode()
	{
		printf("�̸� : %s ���� : %d ���� : %d ���� : %d �� : %d ��� :%0.5f \n", lName, lKor, lEng, lMat, lTotal, lAverage);
		return;
	}
};

template <class T>
class XNode :public Node<T> {
public:
	char lName[20];
	int lKor;
	int lEng;
	int lMat;
	int lTotal;
	float lAverage;
	void PrintNode()
	{
		printf("aaa : %s ���� : %d ���� : %d ���� : %d �� : %d ��� :%0.5f \n", lName, lKor, lEng, lMat, lTotal, lAverage);
		return;
	}
};
