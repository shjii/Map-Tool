#include <string>
#include <vector>
#include <map>
using namespace std;

struct atruck
{
	int index;
	int weight;
	int length;
	bool endtruck = true;
};

int solution(int bridge_length, int weight, vector<int> truck_weights)
{
	int answer = 0;

	vector<atruck> truck;

	for (int a = 0; a < truck_weights.size(); a++)
	{
		atruck t;
		t.index = a;
		t.weight = truck_weights[a];
		t.length = 0;
		truck.push_back(t);
	}

	int index = 0;
	int end = 0;
	int weig = 0;
	while (end != truck_weights.size())
	{
		for (int a = 0; a < truck_weights.size(); a++)
		{
			if (truck[a].endtruck != true) continue;

			if (truck[a].length == 0)
			{
				if (weig + truck[a].weight <= weight)
				{
					truck[a].length++;
					weig += truck[a].weight;
				}
				break;
			}
			else
			{
				truck[a].length++;
				if (truck[a].length == bridge_length)
				{
					weig -= truck[a].weight;
					truck[a].endtruck = false;
					end++;
					if(end <= truck_weights.size()) break;
					if (truck[a + 1].length == 0) break;
				}
			}
		}
		answer++;
	}

	return answer;
}


int main()
{
	vector<int> a = { 7,4,5,6 };
	solution(2, 10, a);
	return true;
}