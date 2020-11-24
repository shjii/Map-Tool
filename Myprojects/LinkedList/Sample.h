#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

typedef struct TStudentNode
{
	char m_szName[20];
	int m_iAge;
	int m_iKor;
	int m_iEng;
	int m_iMat;
	int m_iTotal;
	float m_fAverage;
	TStudentNode* pNext;
}TNode;

TNode* g_pNodeHead = 0;
TNode* g_pNodeTail = 0;
TNode* g_pFindNode = 0;
int g_iNumCounter = 0;

//  �ű� ��� ���� �� ���Ḯ��Ʈ ����
void		NewLink(char* pName, int m_iAge,int* pData = 0);
// �ʱ� ���Ḯ��Ʈ ����
void		RandData();
// pDelNode�� ���Ḯ��Ʈ���� ����
TNode*	DelLink(TNode* pDelNode);
// ��ü ���Ḯ��Ʈ ����
void		AllDeleteLink();
// ���Ϸ� ���� �ε� �� ���Ḯ��Ʈ ����
void		LoadLink(char* pFileName);
// ���Ḯ��Ʈ ��ü�� ���Ϸ� ����.
void		SaveData(char* pSaveFileName);
// pFindName�̸��� ��带 ��ȯ�Ѵ�.
TNode* FindName(char* pFindName);
// pFindNode�� ����Ѵ�.
void PrintData(TNode* pFindNode);
// ��ü ���Ḯ��Ʈ�� ����Ѵ�.
void PrintAllData();
// pFindNode �� ������ ����.
TNode* UpdateData(TNode* pFindNode);
// g_pFindNode ��� �ڿ� �����Ѵ�.
bool InsertLink(TNode* pFindNode, char* pName, int m_iAge, int* pData = 0);
