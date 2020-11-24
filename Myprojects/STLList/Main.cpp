#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <cstdio>
#include <functional>
using namespace std;
class Node
{
public:

	wstring name;

	Node() {}

	Node(wstring _name)
	{
		name = _name;
	}

	bool operator()(Node* _lhs) {
		if (_lhs->name == name)
			return true;
	}
};

// �Լ� ��ü�� Ŭ���� ����
class CFind_Func
{
public:
	CFind_Func(wstring _szname) { m_szName = _szname; }
	~CFind_Func() {}


	bool operator()(Node* _lhs) {
		if (_lhs->name == m_szName)
			return true;
	}
private:
	wstring m_szName;
};


int main()
{
	vector<Node*> myNode;
	vector<Node*>::iterator iteNode;
	int a = 0;
	while (a != 10)
	{
		cout << "1�� ����, 2�� ���, 3�� �˻�" << endl;
		cout << "��ȣ�� �Է��ϼ���." << endl;
		cin >> a;
		switch (a)
		{
		case 1:
		{
			wstring b;
			cout << "�̸� : " << endl;
			wcin >> b;
			myNode.push_back(new Node(b));

		}break;
		case 2:
		{
			for (iteNode = myNode.begin(); iteNode != myNode.end(); iteNode++)
			{
				wcout << " �̸� : " << (*iteNode)->name << endl;
			}
		}break;
		case 3:
		{
			wstring b;
			cout << "�̸� : " << endl;
			wcin >> b;
			iteNode = myNode.begin();
			iteNode = find_if(iteNode, myNode.end(), CFind_Func(b));
			wcout << " �̸� : " << (*iteNode)->name << endl;
			iteNode = myNode.begin();
			iteNode = find_if(iteNode, myNode.end(), Node(b));
			wcout << " �̸� : " << (*iteNode)->name << endl;
		}break;
		default:
			if (a != 10)
			{
				cout << "�߸� �Է� �ϼ̽��ϴ�." << endl;
			}
			break;
		}
	}

	for (iteNode = myNode.begin(); iteNode != myNode.end(); iteNode++)
	{
		vector<Node*>::iterator delNode = iteNode;
		delete *delNode;
	}
	myNode.clear();
}