#include <map>
#include <iostream>
#include <string>
#include <functional>
#include <queue>
#include <vector>
using namespace std;

struct tData
{
	int iValue;
};

template<typename T>
void print_queue(T& q)
{
	while (!q.empty()) {
		cout << q.top() << " ";
		q.pop();
	}
	cout << "\n";
}

struct cmp {
	bool operator()(int a, int b)
	{
		return a < b;
	}

};


int main()
{
	priority_queue<int> q;

	for (int n : {1, 4, 7, 9, 4, 2, 1, 5, 8, 9, 4, 334, 1})
		q.push(n);

	print_queue(q);






	map<string, int > map1;
	map1["½ÅÁöÈÆ"] = 1000;
	map1["°øºÎÇØ"] = 2000;
	map1.insert(make_pair("½ÅÁöÈÆ", 1000));

	map<string, int>::iterator ITOR = map1.find("½ÅÁöÈÆ");
	int iValue = ITOR->second;

	cout << map1["½ÅÁöÈÆ"] << endl;
	cout << map1["°øºÎÇØ"] << endl;

	int m_ilndex = 0;
	map<int, tData> map2;
	tData data0, data1;
	data0.iValue = 123;
	data1.iValue = 459;
	map2[0] = data0;
	map2[1] = data1;
	map2.insert(make_pair(m_ilndex++, data0));

	map<int, tData >::iterator ITOR2 = map2.find(0);
	tData tValue = ITOR2->second;

	for (int i = 0; i < map2.size(); i++)
	{
		std::cout << "key[" << i << "]" << ":" <<
			"data[" << map2[i].iValue << "]" << endl;
	}

	for (std::map<string, int>::iterator itor = map1.begin();
		itor != map1.end();
		itor++)
	{
		string key = itor->first;
		int    data = itor->second;
		std::cout << key << ":" << data << endl;
	}
	for (std::map<int, tData>::iterator itor = map2.begin();
		itor != map2.end();
		itor++)
	{
		int		 key = itor->first;
		tData    data = itor->second;
		std::cout << key << ":" << data.iValue << endl;
	}
	map1.clear();
	return 0;
}