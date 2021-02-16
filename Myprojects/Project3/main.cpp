#include <string>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> solution(vector<int> arr, int divisor) {
	vector<int> answer;

	for (int i = 0; i <= arr.size(); i++)
	{
		if (arr[i] % divisor == 0)
		{
			answer.push_back(arr[i]);
		}
	}
	if (answer.empty()) answer.push_back(-1);
	sort(answer.begin(), answer.end());
	return answer;
}

bool main()
{
	vector<int> arr;
	arr.push_back(5);
	arr.push_back(9);
	arr.push_back(7);
	arr.push_back(10);


	solution(arr, 5);
	return true;
}