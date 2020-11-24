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

// 함수 객체용 클래스 선언
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
		cout << "1번 생성, 2번 출력, 3번 검색" << endl;
		cout << "번호를 입력하세요." << endl;
		cin >> a;
		switch (a)
		{
		case 1:
		{
			wstring b;
			cout << "이름 : " << endl;
			wcin >> b;
			myNode.push_back(new Node(b));

		}break;
		case 2:
		{
			for (iteNode = myNode.begin(); iteNode != myNode.end(); iteNode++)
			{
				wcout << " 이름 : " << (*iteNode)->name << endl;
			}
		}break;
		case 3:
		{
			wstring b;
			cout << "이름 : " << endl;
			wcin >> b;
			iteNode = myNode.begin();
			iteNode = find_if(iteNode, myNode.end(), CFind_Func(b));
			wcout << " 이름 : " << (*iteNode)->name << endl;
			iteNode = myNode.begin();
			iteNode = find_if(iteNode, myNode.end(), Node(b));
			wcout << " 이름 : " << (*iteNode)->name << endl;
		}break;
		default:
			if (a != 10)
			{
				cout << "잘못 입력 하셨습니다." << endl;
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