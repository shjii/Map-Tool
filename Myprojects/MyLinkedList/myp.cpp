#include <cstdio>
#include <cstdlib>
#include <iostream>
struct Node
{
	int iValue;
	Node* pNext;
};

Node *pHeadLink = 0;

void main()
{
	Node* pNewNode = new Node;
	memset(pNewNode, 0, sizeof(Node));
}