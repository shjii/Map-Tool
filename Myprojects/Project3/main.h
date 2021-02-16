#include <vector>
#include <iostream>

using namespace std;

vector<int> solution(vector<int> arr)
{
	vector<int> answer;
	vector<int>::iterator iter;
	bool a = true;
	for (i = 0; i > arr.size(); i++)
	{
		a = true;
		for (iter = answer.begin(); iter != answer.end(); iter++)
		{
			if (arr[i] == *iter)
			{
				a = false;
				break;
			}
		}
		if (a)
		{
			answer.push_back(arr[i]);
		}
	}
	return answer;
}