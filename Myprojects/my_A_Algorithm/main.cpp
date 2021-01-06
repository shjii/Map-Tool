#include <iostream>
#include <vector>
#include <list>


using namespace std;
#define Row 5
#define Col 5

typedef struct Pos
{
	float x = 0;
	float y = 0;
}vPos;

typedef class AStarNode
{
public:

	int m_iParentIndex;
	int m_iNodeIndex;
	bool m_bObstacle;
	vPos m_Pos;
	int m_bG;
	AStarNode()
	{
		m_iParentIndex = 0;
		m_iNodeIndex = 0;
		m_bObstacle = false;
	}
}ANode;

list<int> OpenList;

list<int> CloseList;

vector<vector<ANode*>> vecGraph;
vector<vector<ANode*>>::iterator vec;
vector<ANode*> vecNode;

list<int> PathNode;


bool CheckList(int _iIndex)
{
	for (auto& iIndex : OpenList)
	{
		if (iIndex == _iIndex)
		{
			return true;
		}
	}

	for (auto& iIndex : CloseList)
	{
		if (iIndex == _iIndex)
		{
			return true;
		}
	}

	return false;
}

bool AStarFind(int _iStartIndex, int _iEndIndex)
{
	// 재귀 예외 처리 
	if (25 > _iStartIndex && 0 <= _iStartIndex &&
		25 > _iEndIndex && 0 <= _iEndIndex)
	{
		int iDist = 10000;
		int iNextIndex = 0;
		CloseList.push_back(_iStartIndex);
		vector<ANode*>::iterator iter = vecGraph[_iStartIndex].begin();
		for (iter; iter != vecGraph[_iStartIndex].end(); ++iter)
		{
			if (false == CheckList((*iter)->m_iNodeIndex))
			{
				OpenList.push_back((*iter)->m_iNodeIndex);

				if (0 == (*iter)->m_iParentIndex)
					(*iter)->m_iParentIndex = _iStartIndex;
			}
		}
		for (auto& OpenNum : OpenList)
		{
			int iHx = abs(vecNode[_iEndIndex]->m_Pos.x - vecNode[OpenNum]->m_Pos.x);
			int iHy = abs(vecNode[_iEndIndex]->m_Pos.y - vecNode[OpenNum]->m_Pos.y);
			int iH = iHx + iHy;
			if (10 == iHx && 10 == iHy)
				iH = 14;

			int iGx = abs(vecNode[vecNode[OpenNum]->m_iParentIndex]->m_Pos.x - vecNode[OpenNum]->m_Pos.x);
			int iGy = abs(vecNode[vecNode[OpenNum]->m_iParentIndex]->m_Pos.y - vecNode[OpenNum]->m_Pos.y);
			int iG = iGx + iGy;
			if (10 == iGx && 10 == iGy)
				iG = 14;
			iG += vecNode[vecNode[OpenNum]->m_iParentIndex]->m_bG;
			vecNode[OpenNum]->m_bG = iG;
			int iF = iH + iG;
			if (iF < iDist)
			{
				iDist = iF;
				iNextIndex = OpenNum;
			}
		}
		list<int>::iterator iIter = OpenList.begin();
		for (iIter; iIter != OpenList.end(); ++iIter)
		{
			if (iNextIndex == *iIter)
			{
				OpenList.erase(iIter);
				break;
			}
		}
		if (iNextIndex == _iEndIndex)
		{
			CloseList.push_back(iNextIndex);
			return true;
		}
		return AStarFind(iNextIndex, _iEndIndex);
	}
	else
		return false;
}



void GraphMake()
{
	vPos NodePos;

	int iIndex = 0;

	vecNode.resize(Row * Col);

	for (int i = 0; i < (Row * Col); i++)
	{
		vecNode[i] = new ANode;
		vecNode[i]->m_iNodeIndex = i;

		NodePos.x = (i % 5) * 10.0f;
		NodePos.y = (i / 5) * 10.0f;

		vecNode[i]->m_Pos = NodePos;
	}

	vecNode[6]->m_bObstacle = true;
	vecNode[7]->m_bObstacle = true;
	vecNode[9]->m_bObstacle = true;
	vecNode[15]->m_bObstacle = true;
	vecNode[16]->m_bObstacle = true;
	vecNode[17]->m_bObstacle = true;
	vecNode[18]->m_bObstacle = true;

	vecGraph.resize(Row * Col);

	for (int i = 0; i < Row; i++)
	{
		for (int j = 0; j < Col; j++)
		{
			iIndex = (i * Row) + j;

			if (vecNode[iIndex]->m_bObstacle)
				continue;


			if (0 != (iIndex % Col) && iIndex >= Col) 
			{
				if (false == vecNode[iIndex - Col - 1]->m_bObstacle)
				{
					vecGraph[iIndex].push_back(vecNode[iIndex - Col - 1]);
				}
			}

			if (iIndex >= Col)
			{
				if (false == vecNode[iIndex - Col]->m_bObstacle)
				{
					vecGraph[iIndex].push_back(vecNode[iIndex - Col]);
				}
			}

			if (0 != (iIndex + 1) % Col && iIndex >= Col)
			{
				if (false == vecNode[iIndex - Col + 1]->m_bObstacle)
				{
					vecGraph[iIndex].push_back(vecNode[iIndex - Col + 1]);
				}
			}

			if (0 != (iIndex + 1) % Col)
			{
				if (false == vecNode[iIndex + 1]->m_bObstacle)
				{
					vecGraph[iIndex].push_back(vecNode[iIndex + 1]);
				}
			}

			if (0 != (iIndex + 1) % Col && iIndex < (Row * Col) - Col)
			{
				if (false == vecNode[iIndex + Col + 1]->m_bObstacle)
				{
					vecGraph[iIndex].push_back(vecNode[iIndex + Col + 1]);
				}
			}

			if (iIndex < (Col *  Row) - Col)
			{
				if (false == vecNode[iIndex + Col]->m_bObstacle)
				{
					vecGraph[iIndex].push_back(vecNode[iIndex + Col]);
				}
			}

			if (0 != (iIndex % Col) && iIndex < (Row * Col) - Col)
			{
				if (false == vecNode[iIndex + Col - 1]->m_bObstacle)
				{
					vecGraph[iIndex].push_back(vecNode[iIndex + Col - 1]);
				}
			}

			if (0 != (iIndex % Col))
			{
				if (false == vecNode[iIndex - 1]->m_bObstacle)
				{
					vecGraph[iIndex].push_back(vecNode[iIndex - 1]);
				}
			}
		}
	}
}

int main()
{
	int iStartIndex = 0;
	int iEndIndex = 0;

	cin >> iStartIndex >> iEndIndex;

	GraphMake();

	if (AStarFind(iStartIndex, iEndIndex))
	{
		int iIndex = CloseList.back();
		PathNode.push_front(iIndex);

		while (true)
		{
			iIndex = vecNode[iIndex]->m_iParentIndex;
			PathNode.push_front(iIndex);
			if (iStartIndex == iIndex)
				break;
		}
		PathNode.push_back(iEndIndex);
	}
	else
		cout << "목적지에 도달할 수 없습니다." << endl;


	for (auto& Index : PathNode)
		cout << Index << " ";
	cout << endl;

	return 1;
}